/* Kolmogoroff spectral factorization */
/*
  Copyright (C) 2004 University of Texas at Austin
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <rsf.h>

#include "kolmog.h"

static kiss_fftr_cfg forw, invs;
static int nfft, nw;
static float complex *fft;

void kolmog_init(int n1)
/*< initialize with data length >*/
{
    nfft = n1;
    nw = nfft/2+1;
    fft = sf_complexalloc(nw);   

    forw = kiss_fftr_alloc(nfft,0,NULL,NULL);
    invs = kiss_fftr_alloc(nfft,1,NULL,NULL);
    if (NULL == forw || NULL == invs) 
	sf_error("%s: KISS FFT allocation error");
}

void kolmog_close(void)
/*< free allocated storage >*/
{
    free(fft);
    free(forw);
    free(invs);
}

void kolmog(float *trace)
/*< convert trace to minimum-phase >*/
{
    int i1;

    /* Fourier transform */
    kiss_fftr(forw,trace, (kiss_fft_cpx *) fft);
    for (i1=0; i1 < nw; i1++) {
	trace[i1] = crealf(fft[i1]*conjf(fft[i1]));
    }

    kolmog2(trace);
}

void kolmog2(float *trace)
/*< convert Fourier-domain auto-correlation to minimum-phase >*/ 
{
    int i1;
    const double eps=1.e-32;

    for (i1=0; i1 < nw; i1++) {
	fft[i1] = clog(trace[i1]+eps)/nfft;
    }

    /* Inverse transform */
    kiss_fftri(invs,(const kiss_fft_cpx *) fft, trace);

    trace[0] *= 0.5;
    trace[nfft/2] *= 0.5;
    for (i1=1+nfft/2; i1 < nfft; i1++) {
	trace[i1] = 0.;
    }

    /* Fourier transform */
    kiss_fftr(forw,trace, (kiss_fft_cpx *) fft);
    
    for (i1=0; i1 < nw; i1++) {
	fft[i1] = cexp(fft[i1])/nfft;
    }

    /* Inverse transform */
    kiss_fftri(invs,(const kiss_fft_cpx *) fft, trace);
}

