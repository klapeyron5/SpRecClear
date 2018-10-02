/**
 * ������� ����������� � ����:
 * ad - audio
 * ac - acoustic
 * mod - model
 * _s - structure
 * _t - type
 * _e - enum
 * fh - filehandler
 * dc - decoder
 * feat - feature
 * fe - front-end
 * o - old
 * n - number
 * 
 * raw - 16bit PCM data
 * samples - one instance (16bit sample) of raw data
 */

#include "initializer.h"
#include "wav_rec.h"

/*
 * 
 */
int main() {
	return recognise_from_file(init_dc());
}