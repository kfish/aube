#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "syre.h"
#include "master.h"
#include "syre_if.h"
#include "waves.h"

#include "syre_if.h"
#include "none_config.h"

extern double note_rates[96];

extern bit16 sinwave[SINTAB_LEN];

extern int bpm, beat;
extern int tick;

int syre_if_calc_note(syre_if_data * syre_if);
void restart_beat(syre_if_data * syre_if);
void calc_waveform(syre_if_data * syre_if);
void calc_filter(syre_if_data * syre_if);
void chaos(syre_if_data * syre_if);
void seq_play_once(syre_if_data * syre_if);
void chaos_harmonics(syre_if_data * syre_if);
void clear_harmonics(syre_if_data * syre_if);
void clear_odd_harmonics(syre_if_data * syre_if);

module_class syre_mif = {
	GENERATOR,
	0,
	(char *) "syre",
	(char *) "Syre",
	(void *) syre_if_data_init,
	(void *) syre_new,
	(void *) syre_clone,
	NULL,
	NULL,
	(void *) none_config_cb,
	(void *) syre_if_new,
	(void *) syre_if_close_cb
};

void syre_if_data_init()
{
	generate_sinwave();
	generate_squarewave();
	generate_sawtooth();
	generate_wnwave();
}

syre_if_data *syre_new()
{
	syre_if_data *s;
	int i;

	s = (syre_if_data *) malloc(sizeof(syre_if_data));
	s->module.class = &syre_mif;
	s->module.on = 0;
	s->module.nr_inputs = 1;
	s->module.inputs[0] = malloc(sizeof(input));
	s->module.inputs[0]->ch_type = CH_TYPE_SEQUENCE;
	s->module.inputs[0]->channel = NULL;
	s->module.nr_outputs = 1;
	s->module.outputs[0] = malloc(sizeof(channel));
	s->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
	s->module.outputs[0]->data = (bit16 *) (&(s->tick_buffer));
	aube_module_clear_outputs((module *) s);
	snprintf(s->module.u_label, sizeof(s->module.u_label), "syre");
	snprintf(s->module.outputs[0]->u_label,
		 sizeof(s->module.outputs[0]->u_label), "out");
	s->module.outputs[0]->module = (module *) s;

	s->module.do_tick = (void *) syre_if_calc_note;

	s->wi = 0.0;
	s->bi = 0;
	for (i = 0; i < ENVELOPE_LEN; i++) {
		s->amp_envelope[i] = 32;
	}
	for (i = 0; i < ENVELOPE_LEN; i++) {
		s->pitch_envelope[i] = 32;
	}
	for (i = 0; i < ENVELOPE_LEN; i++) {
		s->amp_envelope_accented[i] = 48;
	}
	for (i = 0; i < ENVELOPE_LEN; i++) {
		s->pitch_envelope_accented[i] = 32;
	}

	for (i = 0; i < ENVELOPE_LEN; i++) {
		s->freq_envelope[i] = 8;
	}
	for (i = 0; i < NUM_HARMONICS; i++) {
		s->overtones[i].si = 0;
	}
	s->tune = 1;
	s->current_wave = sinwave;
	s->cleanbuffer = 1;
	s->harmonics_changed = 1;
	s->env_i = -1;

	chaos_harmonics(s);

	aube_add_module((module *) s);

	return s;
}

syre_if_data *syre_clone(syre_if_data * os)
{
	syre_if_data *s;

	s = (syre_if_data *) malloc(sizeof(syre_if_data));
	memcpy(s, os, sizeof(syre_if_data));

	s->module.nr_inputs = 1;
	s->module.inputs[0] = malloc(sizeof(input));
	s->module.inputs[0]->ch_type = CH_TYPE_SEQUENCE;
	s->module.inputs[0]->channel = os->module.inputs[0]->channel;
	s->module.nr_outputs = 1;
	s->module.outputs[0] = malloc(sizeof(channel));
	s->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
	s->module.outputs[0]->data = (bit16 *) (&(s->tick_buffer));
	aube_module_clear_outputs((module *) s);
	snprintf(s->module.u_label, sizeof(s->module.u_label), "syre");
	snprintf(s->module.outputs[0]->u_label,
		 sizeof(s->module.outputs[0]->u_label), "out");
	s->module.outputs[0]->module = (module *) s;

	s->module.do_tick = (void *) syre_if_calc_note;
	s->current_wave = os->current_wave;

	aube_add_module((module *) s);

	return s;
}

int syre_if_calc_note(syre_if_data * syre_if)
{
	unsigned int i, j, k;
	int spq;		/*
				   samples per tick 
				 */
	int lpan, rpan;
	int amp_env_val;
	int pitch;
	int wip, win;		/*
				   wi prev, next, /2, for averaging filter 
				 */
	float pitch_env_val;
	seq_channel *seq_ch;

	i = 0;

	if (syre_if->module.inputs[0]->channel != NULL) {

		seq_ch =
		    (seq_channel *) syre_if->module.inputs[0]->channel->
		    data;

		if (seq_ch->pan <= 16) {
			lpan = seq_ch->pan;
			rpan = 16;
		} else {
			lpan = 16;
			rpan = 31 - seq_ch->pan;
		}

		if (syre_if->harmonics_changed) {
			calc_waveform(syre_if);
		}
		spq = tick / ENVELOPE_POINTS_PER_TICK;

		if (seq_ch->trigger) {
			syre_if->env_i = 0;
		}
		for (j = 0; j < ENVELOPE_POINTS_PER_TICK; j++) {
			if ((seq_ch->vol > 0) && (syre_if->env_i >= 0)) {

				if (seq_ch->accent) {
					amp_env_val =
					    (int) (((int) syre_if->
						    amp_envelope_accented
						    [syre_if->env_i /
						     TICKS_PER_ENVELOPE_POINT]
						    * seq_ch->vol) >> 6);
					pitch_env_val =
					    syre_if->
					    pitch_envelope_accented
					    [syre_if->env_i /
					     TICKS_PER_ENVELOPE_POINT];
				} else {
					amp_env_val =
					    (int) (((int) syre_if->
						    amp_envelope[syre_if->
								 env_i /
								 TICKS_PER_ENVELOPE_POINT]
						    * seq_ch->vol) >> 6);
					pitch_env_val =
					    syre_if->
					    pitch_envelope[syre_if->env_i /
							   TICKS_PER_ENVELOPE_POINT];
				}


				for (k = 0; k < spq; k += 2) {

#if 0
					if (syre_if->bi < syre_if->port_sub
					    && syre_if->
					    sequence[(syre_if->seqi +
						      SEQ_LENGTH -
						      1) % SEQ_LENGTH].note
					    && syre_if->sequence[syre_if->
								 seqi].
					    portamento_toggle) {
						pitch =
						    (syre_if->port_val *
						     syre_if->
						     sequence[syre_if->
							      seqi].note +
						     (32768 -
						      syre_if->port_val) *
						     syre_if->
						     sequence[(syre_if->
							       seqi +
							       SEQ_LENGTH -
							       1) %
							      SEQ_LENGTH].
						     note) >> 15;
						syre_if->port_val +=
						    syre_if->port_delta;
					} else {
						pitch =
						    syre_if->
						    sequence[seqi].note;
					}
					pitch =
					    (pitch * pitch_env_val) >> 11;
#endif
#if 0
					/*
					   pitch = (seq_ch->pitch*pitch_env_val)>>6;
					 */
					pitch = (seq_ch->pitch * pitch_env_val) >> 2;	/*
											   <<4 for 16*tune 
											 */
#endif

					pitch = seq_ch->pitch * pitch_env_val / 32.0;	/*
											   <<4 for 16*tune 
											 */


					wip =
					    ((int) syre_if->wi +
					     syre_if->waveform_length -
					     pitch / 2) %
					    syre_if->waveform_length;
					win =
					    ((int) syre_if->wi +
					     pitch / 2) %
					    syre_if->waveform_length;

					syre_if->tick_buffer[i] =
					    (bit16) (((syre_if->
						       waveform[(int)
								syre_if->
								wi]) *
						      amp_env_val) >> 6);
#if 0
					(bit16) ((((syre_if->
						    waveform[(int)
							     syre_if->wi] +
						    (syre_if->
						     waveform[wip] +
						     syre_if->
						     waveform[win]) / 2) /
						   2) * amp_env_val) >> 6);
#endif

					syre_if->tick_buffer[i + 1] =
					    (bit16) ((syre_if->
						      tick_buffer[i] *
						      rpan) >> 4);
					syre_if->tick_buffer[i] =
					    (bit16) ((syre_if->
						      tick_buffer[i] *
						      lpan) >> 4);



					syre_if->wi += pitch;
					if (syre_if->wi >
					    (float) syre_if->
					    waveform_length)
						syre_if->wi =
						    syre_if->wi -
						    (float) syre_if->
						    waveform_length;

					i += 2;
				}
			} else {
				for (k = 0; k < spq; k += 2) {
					syre_if->tick_buffer[i] =
					    (bit16) 0;
					syre_if->tick_buffer[i + 1] =
					    (bit16) 0;
					i += 2;
				}
			}

			if (syre_if->env_i > -1) {
				syre_if->env_i++;
				if (syre_if->env_i >
				    (ENVELOPE_LEN *
				     TICKS_PER_ENVELOPE_POINT))
					syre_if->env_i = -1;
			}
		}		/*
				   j 
				 */
	} else {
		for (k = 0; k < tick; k += 2) {
			syre_if->tick_buffer[k] = (bit16) 0;
			syre_if->tick_buffer[k + 1] = (bit16) 0;
		}
	}

	return PROC_SUCCESS;
}

void calc_waveform(syre_if_data * syre_if)
{
	int i, j;
	bit32 d;
/*
   int env_val[NUM_HARMONICS], env_i;
 */

	syre_if->tune = MAX(syre_if->tune, 1);
	syre_if->overtones[0].r = syre_if->tune;
	syre_if->waveform_length = (SINTAB_LEN) / syre_if->overtones[0].r;
	for (i = 1; i < NUM_HARMONICS; i++) {
		syre_if->overtones[i].r = (syre_if->tune * (i + 1));
	}

#if 0
	/*
	   CALCULATE WAVEFORM 
	 */
	for (j = 0; j < NUM_HARMONICS; j++) {
		syre_if->overtones[j].si = 0;
		env_i =
		    (syre_if->sequence[syre_if->seqi].note *
		     syre_if->overtones[j].r) >> 12;
		if (env_i > ENVELOPE_LEN)
			env_i = ENVELOPE_LEN;
		if (env_i < 0)
			env_i = 0;
		env_val[j] = (int) syre_if->freq_envelope[env_i];
	}
#endif

	for (i = 0; i < syre_if->waveform_length; i++) {
		d = 0;
		for (j = 0; j < NUM_HARMONICS; j++) {
			/*
			   d += (syre_if->current_wave[syre_if->overtones[j].si] * env_val[j] *
			   (syre_if->overtones[j].vol-16));
			 */
			d += (syre_if->
			      current_wave[syre_if->overtones[j].si] *
			      (syre_if->overtones[j].vol - 16));
			syre_if->overtones[j].si +=
			    syre_if->overtones[j].r;
			syre_if->overtones[j].si %= SINTAB_LEN;	/*
								   mod length of sinwave 
								 */
		}
		/*
		   syre_if->waveform[i]=d>>12;
		 */
		syre_if->waveform[i] = d >> 7;
	}

	syre_if->harmonics_changed = 0;
}

void chaos_harmonics(syre_if_data * syre_if)
{
	int i;

	for (i = 0; i < NUM_HARMONICS; i++) {
		syre_if->overtones[i].si = 0;
		syre_if->overtones[i].r = 0;
		syre_if->overtones[i].vol =
		    rand() % (32 - (i * 16 / NUM_HARMONICS));
	}
	syre_if->harmonics_changed = 1;
}

void clear_harmonics(syre_if_data * syre_if)
{
	int i;

	for (i = 0; i < NUM_HARMONICS; i++) {
		syre_if->overtones[i].si = 0;
		syre_if->overtones[i].r = 0;
		syre_if->overtones[i].vol = (!i) ? 24 : 16;
	}
	syre_if->harmonics_changed = 1;
}

void clear_odd_harmonics(syre_if_data * syre_if)
{
	int i;

	for (i = 1; i < NUM_HARMONICS; i += 2) {
		syre_if->overtones[i].si = 0;
		syre_if->overtones[i].r = 0;
		syre_if->overtones[i].vol = 16;
	}
	syre_if->harmonics_changed = 1;
}
