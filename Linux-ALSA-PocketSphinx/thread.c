#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <pocketsphinx.h>

#define PS_BUF_SIZE 4096

ps_decoder_t *ps;
cmd_ln_t *config;
ad_rec_t *ad; // Audio Device
int16_t buffer[PS_BUF_SIZE];
int32_t n_samples;
uint8_t utt_started = 0;
uint8_t in_speech;
char const* phrase;

// just used for this example to know when thread is done
int thread_done = 0;

void DictionarySetup() {

  // Create config object with path to language, .lm file, and .dic file
  config = cmd_ln_init(NULL, ps_args(), TRUE,
		       "-hmm", "/usr/local/share/pocketsphinx/model/en-us/en-us",
		       "-lm", "./demo.lm",
		       "-dict", "./demo.dic",
		       NULL);

  if (config == NULL) {
    fprintf(stderr, "Failed to create config object, see log for details\n");
    exit(-1);
  }

  // ps (PocketSphinx) is your main reference value
  ps = ps_init(config);
  if (ps == NULL) {
    fprintf(stderr, "Failed to create recognizer, see log for details\n");
    exit(-1);
  }
}

void* GetPhrase( void *arg ) {

  // plughw:0,2 is capture on dragonbaord by default
  if ((ad = ad_open_dev("plughw:0,2", 16000)) == NULL) {
    fprintf(stderr, "Failed to open audio device\n");
    fflush(stderr);
    exit(-1);
  }

  // start recording
  if (ad_start_rec(ad) < 0) {
    fprintf(stderr, "Failed to start recording\n");
    fflush(stderr);
    exit(-1);
  }

  // start utterance
  if (ps_start_utt(ps) < 0) {
    fprintf(stderr, "Failed to start utterance\n");
    fflush(stderr);
    exit(-1);
  }
 
  // Loop until word is fetched
  while(1) {
    
    // Start getting data
    if ((n_samples = ad_read(ad, buffer, PS_BUF_SIZE)) < 0) {
      fprintf(stderr, "Failed to read audio\n");
      ad_close(ad);      
      fflush(stderr);
      exit(-1);
    }

    ps_process_raw(ps, buffer, n_samples, FALSE, FALSE);
    in_speech = ps_get_in_speech(ps);

    if (in_speech && !utt_started) {
      utt_started = 1;
    }

    if (!in_speech && utt_started) {
      ps_end_utt(ps);
      phrase = ps_get_hyp(ps, NULL);

      if (phrase != NULL) {
	ad_close(ad);
	thread_done = 1;
	pthread_exit(NULL);
	return NULL;
      } else {
	fprintf(stderr, "Phrase was null\n");
	ad_close(ad);	
	fflush(stderr);
	exit(-1);
      }

      if (ps_start_utt(ps) < 0) {
	fprintf(stderr, "Failed to start utterance\n");
	ad_close(ad);
	fflush(stderr);
	exit(-1);
      }
    }
  } // while(1)
}

void CleanUp() {
  ps_free(ps);
  cmd_ln_free_r(config);
}

// used to keep main thread busy
void HeavyCalculations() {

  float accumulator;
  float i;
  float big = 10000000.0f;
  
  while( thread_done == 0 ) {

    for (i = 0.0f; i < big; i++) {
     accumulator += i / (big - i);
    }

    big += 1000.0f;
    
    printf("Accumulator: %.6f\n", accumulator);
  }
}

int main(int argc, char* argv[]) {

  pthread_t voice_thread;
  char* thread_arg = "empty";
  
  DictionarySetup();

  pthread_create( &voice_thread, NULL, GetPhrase, (void*) thread_arg);

  printf("\nSay your phrase...\n\n");
  
  HeavyCalculations();

  printf("\nWhat you said: %s\n\n", phrase);
  
  CleanUp();
}
