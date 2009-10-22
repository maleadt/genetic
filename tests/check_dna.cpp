/*
 * check_dna.cpp
 * Evolve - DNA datastructure test application.
 *
 * Copyright (c) 2009 Tim Besard <tim.besard@gmail.com>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

///////////////////
// CONFIGURATION //
///////////////////

//
// Essential stuff
//

// Headers
#include "../src/dna.h"
#include "../lib/check/check.h"

//
// Constants
//



///////////
// TESTS //
///////////


//
// Auxiliary
//

START_TEST(test_aux_genloc) {
    unsigned char dna1[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03};
    DNA tempDNA1(dna1, 19);

    fail_unless(tempDNA1.gene_start(1) == 1, "Regular gene start index");
    fail_unless(tempDNA1.gene_end(1) == 6, "Regular gene end index");

    fail_unless(tempDNA1.gene_start(0) == 0, "Empty top gene start index");
    fail_unless(tempDNA1.gene_end(0) == 0, "Empty top gene end index");

    fail_unless(tempDNA1.gene_start(2) == 7, "Empty mid gene start index");
    fail_unless(tempDNA1.gene_end(2) == 7, "Empty mid gene end index");
    
    fail_unless(tempDNA1.gene_start(4) == 14, "Regular end gene start index");
    fail_unless(tempDNA1.gene_end(4) == 19, "Regular end gene end index");


    unsigned char dna2[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00};
    DNA tempDNA2(dna2, 19);

    fail_unless(tempDNA2.gene_start(4) == 14, "Regular end gene start index (empty end gene)");
    fail_unless(tempDNA2.gene_end(4) == 19, "Regular end gene end index (empty end gene");
}
END_TEST


//
// Informational
//

START_TEST(test_inf_count) {
    unsigned char dna1[] = {0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA tempDNA1(dna1, 23);
    fail_unless(tempDNA1.genes() == 4, "Gene count");

    unsigned char dna2[] = {0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA tempDNA2(dna2, 24);
    fail_unless(tempDNA2.genes() == 5, "Gene count with empty genes");

    unsigned char dna3[] = {0x00,
    0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04, 0x00};
    DNA tempDNA3(dna3, 26);
    fail_unless(tempDNA3.genes() == 7, "Gene count with empty begin and end");
}
END_TEST


//
// Operators
//

START_TEST(test_op_compare) {
    unsigned char dna1[] = {0x01, 0x02, 0x00, 0x03, 0x02, 0x01, 0x00};
    DNA tempDNA1(dna1, 7);

    unsigned char dna2[] = {0x01, 0x02, 0x00, 0x03, 0x02, 0x01, 0x00};
    DNA tempDNA2(dna2, 7);
    fail_unless(tempDNA1 == tempDNA2, "Normal comparison");

    unsigned char dna3[] = {0x01, 0x04, 0x00, 0x03, 0x02, 0x01, 0x00};
    DNA tempDNA3(dna3, 7);
    fail_unless(tempDNA1 != tempDNA3, "Normal inverse comparison");

    unsigned char dna4[] = {0x01, 0x00, 0x03, 0x02, 0x01, 0x00};
    DNA tempDNA4(dna4, 6);
    fail_unless(tempDNA1 != tempDNA4, "Unequally-sized inverse comparison");
    fail_if(tempDNA1 == tempDNA4, "Unequally-sized inverse comparison");

    unsigned char dna5[] = {0x01, 0x02, 0x00, 0x03, 0x02, 0x01, 0x01};
    DNA tempDNA5(dna5, 7);
    fail_unless(tempDNA1 != tempDNA5, "Normal comparison (corner case)");
}
END_TEST


//
// Modifiers
//

START_TEST(test_mod_erase_simple_start) {
    unsigned char dnastring[] = {0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna = DNA(dnastring, 24);

    dna.erase(0);
    unsigned char dnastring_check[] = {0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna_check(dnastring_check, 18);
    fail_unless(dna_check == dna, "Simple start erase");
}
END_TEST

START_TEST(test_mod_erase_simple_mid) {
    unsigned char dnastring[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna = DNA(dnastring, 25);

    dna.erase(1);
    unsigned char dnastring_check[] = {0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna_check(dnastring_check, 19);
    fail_unless(dna_check == dna, "Simple middle erase");
}
END_TEST

START_TEST(test_mod_erase_simple_end) {
    unsigned char dnastring[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna = DNA(dnastring, 25);

    dna.erase(5);
    unsigned char dnastring_check[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03};
    DNA dna_check(dnastring_check, 19);
    fail_unless(dna_check == dna, "Simple end erase");
}
END_TEST

START_TEST(test_mod_erase_empty_start) {
    unsigned char dnastring[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna = DNA(dnastring, 25);

    dna.erase(0);
    unsigned char dnastring_check[] = {0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna_check(dnastring_check, 24);
    fail_unless(dna_check == dna, "Empty start erase");
}
END_TEST

START_TEST(test_mod_erase_empty_mid) {
    unsigned char dnastring[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna = DNA(dnastring, 25);

    dna.erase(2);
    unsigned char dnastring_check[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna_check(dnastring_check, 24);
    fail_unless(dna_check == dna, "Empty middle erase");
}
END_TEST

START_TEST(test_mod_erase_empty_end) {
    unsigned char dnastring[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04, 0x00};
    DNA dna = DNA(dnastring, 26);

    dna.erase(6);
    unsigned char dnastring_check[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna_check(dnastring_check, 25);
    fail_unless(dna_check == dna, "Empty end erase");
}
END_TEST

START_TEST(test_mod_erase_corner_empty) {
    unsigned char dnastring[] = {0x01, 0x02, 0x03, 0x02, 0x01};
    DNA dna = DNA(dnastring, 5);

    dna.erase(0);
    fail_unless(dna.genes() == 0, "Corner-case erase (removing all genes)");
}
END_TEST

START_TEST(test_mod_insert_front) {
    unsigned char dnastring[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna = DNA(dnastring, 25);

    unsigned char insertstring[] = {0x99, 0x99, 0x99};
    dna.insert(0, insertstring, 3);

    unsigned char dnastring_check[] = {0x99, 0x99, 0x99,
        0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna_check(dnastring_check, 28);
    fail_unless(dna_check == dna, "Front insert");
}
END_TEST

START_TEST(test_mod_insert_mid) {
    unsigned char dnastring[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna = DNA(dnastring, 25);

    unsigned char insertstring[] = {0x99, 0x99, 0x99};
    dna.insert(4, insertstring, 3);

    unsigned char dnastring_check[] = {0x00,
        0x01, 0x02, 0x03,
        0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x99, 0x99, 0x99, 0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna_check(dnastring_check, 28);
    fail_unless(dna_check == dna, "Middle insert");
}
END_TEST

START_TEST(test_mod_pushback) {
    unsigned char dnastring[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA dna = DNA(dnastring, 25);

    unsigned char insertstring[] = {0x99, 0x99, 0x99};
    dna.push_back(insertstring, 3);

    unsigned char dnastring_check[] = {0x00,
        0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04,
        0x99, 0x99, 0x99};
    DNA dna_check(dnastring_check, 28);
    fail_unless(dna_check == dna, "Push back");
}
END_TEST


//
// DNA suite
//


Suite * dna_suite() {
    Suite* s = suite_create("DNA");

    // Auxiliary functionality
    TCase* tc_aux = tcase_create("Auxiliary");
    tcase_add_test(tc_aux, test_aux_genloc);
    suite_add_tcase(s, tc_aux);

    // Informational routines
    TCase* tc_inf = tcase_create("Informational");
    tcase_add_test(tc_inf, test_inf_count);
    suite_add_tcase(s, tc_inf);

    // Operators
    TCase* tc_op = tcase_create("Operators");
    tcase_add_test(tc_op, test_op_compare);
    suite_add_tcase(s, tc_op);

    // Modifiers*/
    TCase* tc_mod = tcase_create("Modifiers");
    tcase_add_test(tc_mod, test_mod_erase_simple_start);
    tcase_add_test(tc_mod, test_mod_erase_simple_mid);
    tcase_add_test(tc_mod, test_mod_erase_simple_end);
    tcase_add_test(tc_mod, test_mod_erase_empty_start);
    tcase_add_test(tc_mod, test_mod_erase_empty_mid);
    tcase_add_test(tc_mod, test_mod_erase_empty_end);
    tcase_add_test(tc_mod, test_mod_erase_corner_empty);
    tcase_add_test(tc_mod, test_mod_insert_front);
    tcase_add_test(tc_mod, test_mod_insert_mid);
    tcase_add_test(tc_mod, test_mod_pushback);
    suite_add_tcase(s, tc_mod);

    return s;
}


//
// Runner
//


int main() {
    int number_failed;
    Suite *s = dna_suite();

    // Run the suite, and be verbose with output
    SRunner* sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);

    // Free resources, and return accordingly
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
