/*
 * octave_code_fir.c
 *
 *  Created on: Dec 12, 2020
 *      Author: fil
 */


/*
close all;
clear all;
clf;
pkg load signal;
pkg load ltfat;
pkg load bsltl;

Fs = 96000;
N = 64
fstart = 4000
fstop  = 5000

start =  [fstart ]/(Fs/2)
stop  =  [fstop  ]/(Fs/2)
freqz ( hc = fir1(N-1, [start,stop]));
freqz(hc);

file_id = fopen('passband_3-5.txt', 'w');
fprintf(file_id,"float32_t ffirCoeffs32[NUM_TAPS] = {\n");
fprintf(file_id,"%.10ff, ",hc);
fprintf(file_id,"\n};\n");
fclose(file_id);




*/
