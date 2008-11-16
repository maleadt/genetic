#!/usr/bin/perl
#
# Genetic.pl
# Test case describing genetic mutations.
#
# Copyright (c) 2008 Tim Besard <tim.besard@gmail.com>
# All rights reserved.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

my $rcsid = '$Id:$';

#################
# CONFIGURATION #
#################

#
# Perl settings
#

# Modules
use strict;
use warnings;
use Class::Struct;

# Settings
my $VERBOSE = 0;
my $EXVERBOSE = 0;
my $VERBOSE_MUTATION = 1;




#
# Data structures
#

# World
struct World => {
	rows => '$',
	columns => '$',
	steps => '$',
	place => '@',
};

# Creature
struct Creature => {
	row => '$',
	column => '$',
	steps => '$',
	DNA => '$',
	genes => '@',
	alive => '$',
};

# DNA structure
#  - Main operators
#    A = if
#    B = if not
#    C = while
#    D = while not (aka unless)
#    E = 
#    F =
my @dnaOperators = qw/A B C D/;

#  - Actions
#    G = move
#    H = 
#    I = 
#    J =
my @dnaActions = qw/G/;

#  - Conditions
#    K = if free
#    L = if blocked
#    M = 
#    N = 
my @dnaConditions = qw/K L/;

#  - Directions
#    O = right
#    P = left
#    Q = up
#    R = down
#    S = right up
#    T = right down
#    U = left up
#    V = left down
my @dnaDirections = qw/O P Q R S T U V/;
	
# Constants
my $constBlocks = 20;
my $constSteps = 500;






##########
## MAIN ##
##########

# CKRGRXCKOGOXALOLRKPGPXCKRGRX, evolueert naar mooie 66%
# CKRGRXCKOGOXALOLRKTGTX, ook mooie evolutie
# CKOGOXCKRGRXAKTGTX, bijzonder mooie evolutie

# 12h -> CKTKTKTGTGTGTGTXCKOKOGOXCKRGRXCKTKTKTGTGTGTGTXCKRGRXCKRKRGRXCKRGRX -> dups -> CKTGTXCKOGOXCKRGRXCKTGTXCKRGRX


# Base DNA
my $dataDNA = "CKRGRXCKOGOXALOLRKPGPXCKRGRX";
my $dataRuns = 100;

# Previous settings
my $prevDNA = $dataDNA;
my $prevPerc = -1;
my $prevDist = -1;

# Esthetical thingie
my $tempSpace = 50;

# Start simulation
print "- Starting simulation\n";
while (1)
{
	print "Current max: $prevDNA, with $prevPerc\% rate\n";
	# Test 100 world to see a success rate
	my $tempSuccess = 0;
	my $tempSteps = 0;
	
	# Partial output
	$tempSpace *= 2 if ($tempSpace-2 < length($dataDNA));
	print "\t$dataDNA", " "x($tempSpace-length($dataDNA));
	
	# Simulation
	for (0 ... $dataRuns)
	{
		# Create a new world
		my $dataWorld = newWorld(20, 20);
		my $dataCreature = newCreature(\$dataWorld, $dataDNA);
		
		# Check validity
		if (! $dataCreature->alive)
		{
			print "non functional\n";
			last;
		}
		
		# Run the simulation
		Simulate(\$dataWorld, \$dataCreature);

		# Save results
		if ($dataCreature->steps < $dataWorld->steps)
		{
			$tempSuccess++;
			$tempSteps += $dataCreature->steps;
		}
	}

	# Print results
	if ($tempSteps > 0)
	{
		my $tempSuccessSteps = 0;
		$tempSuccessSteps = int($tempSteps/$tempSuccess) if ($tempSuccess != 0);
		print int(100 * $tempSuccess/$dataRuns), "%, avg ", $tempSuccessSteps, " steps\n";
	}
	else
	{
		print "non functional (incorrectly reported)\n";
	}
	
	# Compare with previous one
	if ($prevPerc > int(100 * $tempSuccess/$dataRuns))
	{
		$dataDNA = $prevDNA;
		print "* Not efficient, reverting\n" if ($VERBOSE);
	} else {
		print "* Efficient, saving!\n" if ($VERBOSE);
		$prevDNA = $dataDNA;
		$prevPerc = int(100 * $tempSuccess/$dataRuns);
		$prevDist = int($tempSteps/$tempSuccess) if ($tempSuccess > 0);
		$prevDist = -1 if ($tempSuccess == 0);
	}
	
	# Mutate!
	my $rand = rand(10);
	$dataDNA = mutate($dataDNA) if ($rand < 2);
	$dataDNA = mutate($dataDNA) if ($rand < 5);
	$dataDNA = mutate($dataDNA) if ($rand < 10);
}

exit;



############
# ROUTINES #
############

#
# Object generation
#

# Generate a new world
#  input: amount of rows (scalar), amount of columns (scalar)
#  output: world (world structure)
sub newWorld
{
	# World data structure
	#  0 = free spot
	#  1 = occupied spot
	#  B = start spot
	#  E = end spot
	#  X = place of the creature
	
	# Input
	my $inputRows = shift;
	my $inputColumns = shift,
	my $inputBlocks = shift;
	

	# Initialise
	my $outputWorld = World->new();
	$outputWorld->rows($inputRows);
	$outputWorld->columns($inputColumns);
	$outputWorld->steps($constSteps);

	# Fill
	for (my $i = 0; $i < $outputWorld->rows; $i++)
	{
		for (my $j = 0; $j < $outputWorld->columns; $j++)
		{
			$outputWorld->place->[$i][$j] = 0;
		}
	}
	# Set start and end spot
	$outputWorld->place->[0][0] = "B";
	$outputWorld->place->[$outputWorld->rows-1][$outputWorld->columns-1] = "E";

	# Set 20% blocks
	for (my $i = 0; $i < ($outputWorld->rows * $outputWorld->columns)*$constBlocks/100; $i++)
	{
		# Generate random but valid coördinates
		my $tempRow = 0;
		my $tempColumn = 0;
		while ( ( $tempRow == 0 && $tempColumn == 0 ) || ( $tempRow == ($outputWorld->rows-1) && $tempColumn == ($outputWorld->columns-1) ) )
		{
			$tempRow = int(rand($outputWorld->rows));
			$tempColumn = int(rand($outputWorld->columns));
		}
	
		# Set block
		$outputWorld->place->[$tempRow][$tempColumn] = 1;
	}

	return $outputWorld;
}


# Generate a new creature
#  input: world (world structure), DNA code (string)
#  output: creature (creature object)
sub newCreature
{
	# Input
	my $inputWorldRef = shift;
	my $inputWorld = $$inputWorldRef;
	my $inputDNA = shift;

	# Initialise
	my $outputCreature = Creature->new;
	$outputCreature->DNA($inputDNA);
	$outputCreature->alive(1);

	# Place the creature on the world map (search for the B spot)
	for (my $i = 0; $i<$inputWorld->rows && !$outputCreature->row; $i++)
	{
		for (my $j = 0; $j<$inputWorld->columns && !$outputCreature->column; $j++)
		{
			if ($inputWorld->place->[$i][$j] eq "B")
			{
				$inputWorld->place->[$i][$j] = "X";
				$outputCreature->row($i);
				$outputCreature->column($j);
			}
		}
	}

	# Split it's DNA into genes
	for (my $i = 0; $i < length($outputCreature->DNA); $i++)
	{
		# We have a primary gene
		if (isOperator(substr($outputCreature->DNA, $i, 1)))
		{
			my $j = find_first_of($outputCreature->DNA, "X", $i);
			push(@{$outputCreature->genes}, substr($outputCreature->DNA, $i, $j-$i));
			$i = $j;
		}
	
		# If not, this is an invalid DNA
		else
		{
			print "! Invalid DNA sequence (element out of gene).\n" if ($VERBOSE);
			$outputCreature->alive(0);
			last;
		}
	
		# Nested genes = BAD (read: not supported)
		if ($outputCreature->genes->[-1] =~ m/((A|B|C)(.*)){2,}/)
		{
			print "! Invalid DNA sequence (nested genes).\n" if ($VERBOSE);
			$outputCreature->alive(0);
			last;
		}
	}
	print "* Broke DNA up in " . scalar(@{$outputCreature->genes}) . " genes.\n" if ($EXVERBOSE);
	
	return $outputCreature;
}

#
# Genetic routines
#

# Mutate a given DNA string
#   input: DNA (string)
#   output: DNA (string)
sub mutate
{
	# Input
	my $inputDNA = shift;
	my $outputDNA = $inputDNA;	# In case we don't mutate
	
	# Random factor
	my $random = rand(3);
	
	#
	# Point mutation
	#
	
	if ($random < 1)
	{
		print "\tpoint mutation: " if ($VERBOSE_MUTATION);
		
		# Pick random element
		my $tempElementIndex;
		my $tempElement = "X";
		while ($tempElement eq "X")
		{
			$tempElementIndex = int(1 + rand(length($inputDNA)-1));
			$tempElement = substr($inputDNA, $tempElementIndex, 1);
		}
		my $tempMutated = $tempElement;	# In case we don't find anything
		
		# Switch it
		if (isOperator($tempElement))
		{
			$tempMutated = rand_el(\@dnaOperators);
		}
		elsif(isCondition($tempElement))
		{
			$tempMutated = rand_el(\@dnaConditions);
		}
		elsif(isAction($tempElement))
		{
			$tempMutated = rand_el(\@dnaActions);
		}
		elsif(isDirection($tempElement))
		{
			$tempMutated = rand_el(\@dnaDirections);
		}
		
		print "switch $tempElement with $tempMutated\n" if ($VERBOSE_MUTATION);
		$outputDNA = substr($inputDNA, 0, $tempElementIndex) . $tempMutated . substr($inputDNA, $tempElementIndex+1, length($inputDNA)-$tempElementIndex);
	}
	
	#
	# Chromsone mutation
	#
	
	elsif ($random < 2)
	{
		print "\tchromsone mutation: " if ($VERBOSE_MUTATION);
		
		#
		# Initialize
		#
		
		# Split DNA into genes
		my @tempGenes;
		for (my $i = 0; $i < length($inputDNA); $i++)
		{
			# We have a primary gene
			if (isOperator(substr($inputDNA, $i, 1)))
			{
				my $j = find_first_of($inputDNA, "X", $i);
				push(@tempGenes, substr($inputDNA, $i, $j-$i));
				$i = $j;
			}
		}
		
		# Random value, to choose between inversion, deletion, translocation, duplication, fusion or fission
		my $tempRandom = int(rand(5));
		
		# If we only have one gene, we can only apply fusion or duplication
		$tempRandom = int(rand(2)) if ($#tempGenes == 0);
		
		# Random indexes
		my $tempRandom1 = int(rand($#tempGenes+1));
		my $tempRandom2 = int(rand($#tempGenes+1));
		while ($tempRandom1 == $tempRandom2)
		{
			$tempRandom2 = int(rand($#tempGenes+1));
			last if ($#tempGenes == 0);
		}
		if ($tempRandom1 > $tempRandom2)
		{
			my $temp = $tempRandom1;
			$tempRandom1 = $tempRandom2;
			$tempRandom2 = $temp;
		}
		
		#
		# Mutate
		#
		
		# Fusion
		if ($tempRandom < 1)
		{
			# Process gene
			my $tempOperator = substr($tempGenes[$tempRandom1], 0, 1);
			my $tempCondition = "";
			my $i = 1;
			while ($i<length($tempGenes[$tempRandom1]) && isCondition(substr($tempGenes[$tempRandom1], $i, 1)))
			{
				$tempCondition = $tempCondition . substr($tempGenes[$tempRandom1], $i, 2);
				$i+=2;
			}
			my $tempAction = substr($tempGenes[$tempRandom1], $i, length($tempGenes[$tempRandom1])-$i);
			
			# Split condition
			my $tempCondition1 = $tempCondition;
			my $tempCondition2 = $tempCondition;
			if (length($tempCondition) > 2)
			{
				my $split = 2 * int(length($tempCondition)/4);
				$tempCondition1 = substr($tempCondition, 0, $split);
				$tempCondition2 = substr($tempCondition, $split, length($tempCondition)-$split);
			}
			
			# Split actions
			my $tempAction1 = $tempAction;
			my $tempAction2 = $tempAction;
			if (length($tempAction) > 2)
			{
				my $split = 2 * int(length($tempAction)/4);
				$tempAction1 = substr($tempAction, 0, $split);
				$tempAction2 = substr($tempAction, $split, length($tempAction)-$split);
			}
			
			# Save genes
			print "fusion of gene $tempRandom1\n" if ($VERBOSE_MUTATION);
			$tempGenes[$tempRandom1] = $tempOperator . $tempCondition1 . $tempAction1;
			push(@tempGenes, $tempOperator . $tempCondition2 . $tempAction2);
		}
		
		# Duplication
		elsif ($tempRandom < 2)
		{
			push @tempGenes, $tempGenes[-1];
			for (my $i = $#tempGenes; $i>=$tempRandom2; $i--)
			{
				$tempGenes[$i+1] = $tempGenes[$i];
			}
			
			# Save gene
			print "fusion of gene $tempRandom1\n" if ($VERBOSE_MUTATION);
			$tempGenes[$tempRandom2] = $tempGenes[$tempRandom1];
		}
		
		# Inversion
		elsif ($tempRandom < 3)
		{
			my $tempGene = $tempGenes[$tempRandom1];
			$tempGenes[$tempRandom1] = $tempGenes[$tempRandom2];
			$tempGenes[$tempRandom2] = $tempGene;
			
			print "inversion of genes $tempRandom1 and $tempRandom2\n" if ($VERBOSE_MUTATION);
				
		}
		
		# Deletion
		elsif ($tempRandom < 4)
		{
			for (my $i = $tempRandom1+1; $i <= $#tempGenes; $i++)
			{
				$tempGenes[$i-1] = $tempGenes[$i];
			}
			pop @tempGenes;
			
			print "deletion of gene $tempRandom1\n" if ($VERBOSE_MUTATION);
		}
		
		# Fission
		elsif ($tempRandom < 5)
		{
			# Process gene 1
			my $tempOperator1 = substr($tempGenes[$tempRandom1], 0, 1);
			my $tempCondition1 = "";
			my $i = 1;
			while ($i<length($tempGenes[$tempRandom1]) && isCondition(substr($tempGenes[$tempRandom1], $i, 1)))
			{
				$tempCondition1 = $tempCondition1 . substr($tempGenes[$tempRandom1], $i, 2);
				$i+=2;
			}
			my $tempAction1 = substr($tempGenes[$tempRandom1], $i, length($tempGenes[$tempRandom1])-$i);
			
			# Process gene 2
			my $tempOperator2 = substr($tempGenes[$tempRandom2], 0, 1);
			my $tempCondition2 = "";
			$i = 1;
			while ($i<length($tempGenes[$tempRandom2]) && isCondition(substr($tempGenes[$tempRandom2], $i, 1)))
			{
				$tempCondition2 = $tempCondition2 . substr($tempGenes[$tempRandom2], $i, 2);
				$i+=2;
			}
			my $tempAction2 = substr($tempGenes[$tempRandom2], $i, length($tempGenes[$tempRandom2])-$i);
			
			# Merge them
			my $tempOperator;
			if (rand(2) < 1)
			{
				$tempOperator = $tempOperator1;
			} else {
				$tempOperator = $tempOperator2;
			}
			my $tempCondition = $tempCondition1 . $tempCondition2;
			my $tempAction = $tempAction1 . $tempAction2;
			
			# Save new gene
			$tempGenes[$tempRandom1] = $tempOperator . $tempCondition . $tempAction;
			
			# Delete old gene
			for (my $i = $tempRandom2+1; $i <= $#tempGenes; $i++)
			{
				$tempGenes[$i-1] = $tempGenes[$i];
			}
			pop @tempGenes;
			
			print "fission of genes $tempRandom1 and $tempRandom2\n" if ($VERBOSE_MUTATION);
		}
		
		#
		# Finish
		#
		
		# Reconstruct gene
		$outputDNA = "";
		for (my $i = 0; $i <= $#tempGenes; $i++)
		{
			$outputDNA .= $tempGenes[$i] . 'X';
		}
	}
	
	# Gen mutations
	elsif ($random < 3)
	{
		print "\tgen mutations: " if ($VERBOSE_MUTATION);
		
		#
		# Initialize
		#
		
		# Split DNA into genes
		my @tempGenes;
		for (my $i = 0; $i < length($inputDNA); $i++)
		{
			# We have a primary gene
			if (isOperator(substr($inputDNA, $i, 1)))
			{
				my $j = find_first_of($inputDNA, "X", $i);
				push(@tempGenes, substr($inputDNA, $i, $j-$i));
				$i = $j;
			}
		}
		
		# Pick a gene
		my $tempRandom = int(rand($#tempGenes+1));
		
		# Process that gene
		my $tempOperator = substr($tempGenes[$tempRandom], 0, 1);
		my $tempCondition = "";
		my $i = 1;
		while ($i<length($tempGenes[$tempRandom]) && isCondition(substr($tempGenes[$tempRandom], $i, 1)))
		{
			$tempCondition = $tempCondition . substr($tempGenes[$tempRandom], $i, 2);
			$i+=2;
		}
		my $tempAction = substr($tempGenes[$tempRandom], $i, length($tempGenes[$tempRandom])-$i);
		
		#
		# Mutate
		#
		
		my $tempRandom2 = rand(1);
		
		# Duplication of ccodon
		if ($tempRandom2 < 1)
		{
			my $tempRandom3 = rand(2);
			
			if ($tempRandom3 < 1)
			{
				# Duplicate a condition codon (place it in the middle)
				my $randCodon = 2 * int(length($tempCondition)/4);
				$randCodon+=2 if ($randCodon == 0);
				$tempCondition = substr($tempCondition, 0, $randCodon) . substr($tempCondition, $randCodon-2, 2) . substr($tempCondition, $randCodon, length($tempCondition)-$randCodon);
			}
			elsif ($tempRandom3 < 2)
			{			
				# Duplicate an action codon (place it in the middle)
				my $randCodon = 2 * int(length($tempAction)/4);
				$randCodon+=2 if ($randCodon == 0);
				$tempAction = substr($tempAction, 0, $randCodon) . substr($tempAction, $randCodon-2, 2) . substr($tempAction, $randCodon, length($tempAction)-$randCodon);
			}
			
			$tempGenes[$tempRandom] = $tempOperator . $tempCondition . $tempAction;
			print "duplicating codon\n" if ($VERBOSE_MUTATION);
		}
		
		# Inverse of codon
		#TODO: werken met cases? (case 1, case 2, ...)
		elsif ($tempRandom2 < 2)
		{
			my $tempRandom3 = rand(2);
			
			if ($tempRandom3 < 1)
			{
				# Duplicate a condition codon (place it in the middle)
				my $randCodon = 2 * int(length($tempCondition)/4);
				$randCodon+=2 if ($randCodon == 0);
				$tempCondition = substr($tempCondition, 0, $randCodon-2) . substr($tempCondition, $randCodon, 2) . substr($tempCondition, $randCodon-2, 2) . substr($tempCondition, $randCodon+2, length($tempCondition)-$randCodon);
			}
			elsif ($tempRandom3 < 2)
			{			
				# Duplicate an action codon (place it in the middle)
				my $randCodon = 2 * int(length($tempAction)/4);
				$randCodon+=2 if ($randCodon == 0);
				$tempAction = substr($tempAction, 0, $randCodon-2) . substr($tempAction, $randCodon, 2) . substr($tempAction, $randCodon-2, 2) . substr($tempAction, $randCodon+2, length($tempAction)-$randCodon);
			}
			
			$tempGenes[$tempRandom] = $tempOperator . $tempCondition . $tempAction;
			print "inversing codon\n" if ($VERBOSE_MUTATION);
		}
		
		#
		# Finish
		#
		
		# Reconstruct gene
		$outputDNA = "";
		for (my $i = 0; $i <= $#tempGenes; $i++)
		{
			$outputDNA .= $tempGenes[$i] . 'X';
		}
	}
	
	return $outputDNA;
}

sub isOperator
{
	# Input
	my $inputChar = shift;
	
	# Check
	if (index_of(\@dnaOperators, $inputChar) != -1)
	{
		return 1;
	}
	
	return 0;
}

sub isCondition
{
	# Input
	my $inputChar = shift;
	
	# Check
	if (index_of(\@dnaConditions, $inputChar) != -1)
	{
		return 1;
	}
	
	return 0;
}

sub isAction
{
	# Input
	my $inputChar = shift;
	
	# Check
	if (index_of(\@dnaActions, $inputChar) != -1)
	{
		return 1;
	}
	
	return 0;
}

sub isDirection
{
	# Input
	my $inputChar = shift;
	
	# Check
	if (index_of(\@dnaDirections, $inputChar) != -1)
	{
		return 1;
	}
	
	return 0;
}

# Run a simulation of a creature in a given world
#  input: world (world structure), creature (creature structure)
#  output: amount of steps (scalar)
sub Simulate
{
	# Input
	my $inputWorldRef = shift;
	my $inputWorld = $$inputWorldRef;
	my $inputCreatureRef = shift;
	my $inputCreature = $$inputCreatureRef;
	
	# Parse creature DNA
	$inputCreature->steps(0);
	for (; $inputCreature->steps < $inputWorld->steps;)
	{
		# Loop every gene
		foreach my $tempGene (@{$inputCreature->genes})
		{
			print "* Processing gene $tempGene\n" if ($EXVERBOSE);
		
			# Every gene consists out of three parts: operator - condition - action
			my $tempOperator = substr($tempGene, 0, 1);
			my $tempCondition = "";
			my $i = 1;
			while ($i<length($tempGene) && isCondition(substr($tempGene, $i, 1)))
			{
				$tempCondition = $tempCondition . substr($tempGene, $i, 2);
				$i+=2;
			}
			my $tempAction = substr($tempGene, $i, length($tempGene)-$i);
					
			# We got an "if" operator
			if ($tempOperator eq "A")
			{
				if (codonCondition(\$inputWorld, \$inputCreature, $tempCondition))
				{
					codonAction(\$inputWorld, \$inputCreature, $tempAction);
				}
			}
		
			# We got a "while" operator
			if ($tempOperator eq "C")
			{
				while (codonCondition(\$inputWorld, \$inputCreature, $tempCondition) && $inputCreature->steps < $inputWorld->steps)
				{				
					my $return = codonAction(\$inputWorld, \$inputCreature, $tempAction);
					if ($return == 2)
					{
						return $inputCreature->steps;
					}
				}
			}
		}
	}
	
	return -1;
}



# Evaluate a condition codon
#  input: codon (string)
#  output: result (bool, 1 or 0)
sub codonCondition
{
	# Input
	my $inputWorldRef = shift;
	my $inputWorld = $$inputWorldRef;
	my $inputCreatureRef = shift;
	my $inputCreature = $$inputCreatureRef;
	my $inputCondition = shift;
	print "* Got condition codon: $inputCondition\n" if ($EXVERBOSE);
	
	# Parse every sub-condition
	while ($inputCondition =~ s/^(..)//)
	{
		# Split the condition
		my $tempSingleCondition = $1;
	
		# Split the condition in a "test" and it's "parameter"
		my $tempTest = substr($tempSingleCondition, 0, 1);
		my $tempParameter = substr($tempSingleCondition, 1, 1);
		
		# Increase the counter
		$inputCreature->steps($inputCreature->steps + 1);
		
		# We got a "check if free"-test
		if ($tempTest eq "K")
		{
			# Look up new coördinates and safety of moving to it
			my ($tempSafe, $tempRow, $tempColumn) = spotMove($inputWorld, $inputCreature, $tempParameter);
			
			# Safe to move
			if ($tempSafe == 1)
			{
				next
			}
			
			# Not safe to move (occupied or out-of-range)
			else
			{
				return 0;
			}
		}
		
		# We got a "check if blocked"-test
		if ($tempTest eq "L")
		{
			# Look up new coördinates and safety of moving to it
			my ($tempSafe, $tempRow, $tempColumn) = spotMove($inputWorld, $inputCreature, $tempParameter);
			
			# Free
			if ($tempSafe == 1)
			{
				return 0;
			}
			
			# Not safe to move (occupied or out-of-range)
			else
			{
				next;
			}
		}
	}
	
	return 1;
}

# Execute an action codon
#  input: codon (string)
sub codonAction
{
	# Input
	my $inputWorldRef = shift;
	my $inputWorld = $$inputWorldRef;
	my $inputCreatureRef = shift;
	my $inputCreature = $$inputCreatureRef;
	my $inputAction = shift;
	print "* Got action codon: $inputAction\n" if ($EXVERBOSE);
	
	# Parse every sub-action
	while ($inputAction =~ s/^(..)//)
	{
		my $tempSingleAction = $1;
	
		# Split the condition in an "action" and it's "parameter"
		my $tempAction = substr($tempSingleAction, 0, 1);
		my $tempParameter = substr($tempSingleAction, 1, 1);
		
		# Increase the counter
		$inputCreature->steps($inputCreature->steps + 1);
		
		# We got a "move to"-action
		if ($tempAction eq "G")
		{
			# Look up new coördinates and safety of moving to it
			my ($tempSafe, $tempRow, $tempColumn) = spotMove($inputWorld, $inputCreature, $tempParameter);
			
			# Free
			if ($tempSafe == 1)
			{
				# Spot is free, move to it
				$inputWorld->place->[$inputCreature->row][$inputCreature->column] = "0";
				$inputCreature->row($tempRow);
				$inputCreature->column($tempColumn);
			
				# Are we at the end?
				if ($inputWorld->place->[$inputCreature->row][$inputCreature->column] eq "E")
				{
					return 2;
				}
			
				# Not yet
				$inputWorld->place->[$inputCreature->row][$inputCreature->column] = "X";
				next;
			}
			
			# Not safe to move (occupied or out-of-range)
			else
			{
				print "! Attempt to move at an invalid spot\n" if ($VERBOSE);
				$inputCreature->alive(0);
				return 0;
			}			
		}
	}
	
	return 1;
}


#
# Coördinate routines
#

# Calculate the new coördinates given on a parameter, and the safety of moving to it
#  input: parameter (string, R L U or D)
#  output: safety (scalar, -1 for out-of-range, 0 for blocked, 1 for safe), new row (scalar), new column (scalar)
sub spotMove
{
	# Input values
	my $inputWorld = shift;
	my $inputCreature = shift;
	my $inputParameter = shift;
	
	# Check direction
	my $deltaRow = 0;
	my $deltaColumn = 0;
	$deltaColumn++ if ($inputParameter eq "O"  ||  $inputParameter eq "S"  ||  $inputParameter eq "T");
	$deltaColumn-- if ($inputParameter eq "P"  ||  $inputParameter eq "U"  ||  $inputParameter eq "V");
	$deltaRow-- if    ($inputParameter eq "Q"  ||  $inputParameter eq "S"  ||  $inputParameter eq "U");
	$deltaRow++ if    ($inputParameter eq "R"  ||  $inputParameter eq "T"  ||  $inputParameter eq "V");
	
	# Generate a new spot
	my $outputRow = $inputCreature->row+$deltaRow;
	my $outputColumn = $inputCreature->column+$deltaColumn;
	
	# Check validity of spot
	if ($outputRow<0 || $outputColumn<0 || $outputRow>=$inputWorld->rows
		|| $outputColumn>=$inputWorld->columns)
	{
		return (-1, 0, 0);
	}
	
	# Check occupation of spot
	if ($inputWorld->place->[$outputRow][$outputColumn] !~ m/(0|B|E)/i)
	{
		return (0, 0, 0);
	}
	
	# Return new coördinates
	return (1, $outputRow, $outputColumn);
}




# Display the current world
#  input: world (world struct)
sub outputWorld
{
	# Input
	my $inputWorld = shift;
	
	# Display it
	for (my $i = 0; $i < $inputWorld->rows; $i++)
	{
		print "\t";
		for (my $j = 0; $j < $inputWorld->columns; $j++)
		{
			print $inputWorld->place->[$i][$j], "\t";
		}
		print "\n";
	}
}



#
# Other routines
#

# Find the first occurence of a given string
#  input: string to search in (string), character to look for (string), offset to start looking (scalar)
#  output: resulting place (scalar)
sub find_first_of
{
	# Input
	my $inputString = shift;
	my $inputChar = shift;
	my $inputOffset = shift | 0;
	
	# Look for inputChar
	for (my $i = $inputOffset; $i < length($inputString); $i++)
	{
		if (substr($inputString, $i, 1) eq $inputChar)
		{
			return $i;
		}
	}
	
	# Not found
	return -1;
}

sub index_of
{
	# Input
	my $inputArrayRef = shift;
	my @inputArray = @$inputArrayRef;
	my $inputElement = shift;
	
	# Loop them all
	for (my $i = 0; $i < scalar(@inputArray); $i++)
	{
		return $i if ($inputArray[$i] eq $inputElement);
	}
	# Not found
	return -1;
}

sub rand_el
{
	# Input
	my $inputArrayRef = shift;
	my @inputArray = @$inputArrayRef;
	
	# Pick random element
	my $randomIndex = rand($#inputArray);
	return $inputArray[$randomIndex];
}

__END__
