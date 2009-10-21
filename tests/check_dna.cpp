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

START_TEST(test_dna_count) {
    unsigned char dna1[] = {0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA tempDNA1(dna1, 23);
    fail_unless(tempDNA1.genes() == 4, "Amount of genes not counted correctly");

    unsigned char dna2[] = {0x00, 0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04, 0x00};
    DNA tempDNA2(dna2, 26);

    fail_unless(tempDNA2.genes() == 4, "Amount of genes not counted correctly when empty genes occur");
}
END_TEST

START_TEST(test_dna_compare) {
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

START_TEST(test_dna_erase) {
    unsigned char dna1[] = {0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x02, 0x03, 0x04, 0x03, 0x02, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA tempDNA1(dna1, 23);
    tempDNA1.erase(1);
    unsigned char dna1_check[] = {0x01, 0x02, 0x03, 0x02, 0x01, 0x00,
        0x03, 0x04, 0x05, 0x04, 0x03, 0x00,
        0x04, 0x05, 0x06, 0x05, 0x04};
    DNA tempDNA1_check(dna1_check, 17);
    fail_unless(tempDNA1 == tempDNA1_check, "Simple erase");
    // TODO: toch iet doen met lege genes, 0x00 ALTIJD laten meecounten voor next gene
    // anders lyk hoe erasen, lege gene?
}
END_TEST


Suite * dna_suite() {
    Suite* s = suite_create("DNA");

    /* Core test case */
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_dna_count);
    tcase_add_test(tc_core, test_dna_compare);
    tcase_add_test(tc_core, test_dna_erase);
    suite_add_tcase(s, tc_core);

    return s;
}


int main() {
    int number_failed;
    Suite *s = dna_suite();

    // Run the suite, and be verbose with output
    SRunner* sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);

    // Free resources, and return correectly
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
