#pragma once

// Trigonometric constants
const double deg_to_rad = 0.01745329251994;
const double rad_to_deg = 57.2957795131;
const double pi = 3.14159265358979323846;
const double pi_2 = 1.570796326795;
const double pi_3 = 1.0471975511965;
const double pi_4 = 0.7853981633973;
const double pi_6 = 0.523598775598;
const double cos45_sin45 = 0.707106781186547524;

// Euler's number, e
const double euler_num = 2.7182818284590452353602874;

// Factors
const double base_to_tera = 0.000000000001;
const double base_to_giga = 0.000000001;
const double base_to_mega = 0.000001;
const double base_to_kilo = 0.001;
const double base_to_hecto = 0.01;
const double base_to_deka = 0.1;
const double base_to_base = 1.0;
const double base_to_deci = 10.0;
const double base_to_centi = 100.0;
const double base_to_milli = 1000.0;
const double base_to_100_micro = 10000.0;
const double base_to_micro = 1000000.0;
const double base_to_nano = 1000000000.0;
const double base_to_pico = 1000000000000.0;
const double centi_cubed_to_base_cubed = 0.000001;

// Time Conversions, 
const double msec_to_sec = 0.001;
const double sec_to_msec = 1000.0;
const double min_to_sec = 60.0;
const double hr_to_min = 60.0;
const double day_to_hr = 24.0;
const double day_to_msec = day_to_hr * hr_to_min * min_to_sec * sec_to_msec;
const double day_to_sec = day_to_hr * hr_to_min * min_to_sec;
const double day_to_min = day_to_hr * hr_to_min;
const double hr_to_msec = hr_to_min * min_to_sec * sec_to_msec;
const double min_to_msec = min_to_sec * sec_to_msec;