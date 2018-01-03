#include <stdio.h>
#include <stdint.h>
#include <stdint.h>
#include <string.h>

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

// forward delcarion
void Add(char const* phrase);

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

void GetPhrase() {

  // plughw:0,2 is capture on dragonbaord by default
  if ((ad = ad_open_dev("plughw:0,2", 16000)) == NULL) {
    fprintf(stderr, "Failed to open audio device\n");
    exit(-1);
  }

  // start recording
  if (ad_start_rec(ad) < 0) {
    fprintf(stderr, "Failed to start recording\n");
    return;
  }

  // start utterance
  if (ps_start_utt(ps) < 0) {
    fprintf(stderr, "Failed to start utterance\n");
    return;
  }

  printf("\nSay your phrase...\n\n");
  
  // Loop until word is fetched
  while(1) {
    
    // Start getting data
    if ((n_samples = ad_read(ad, buffer, PS_BUF_SIZE)) < 0) {
      fprintf(stderr, "Failed to read audio\n");
      ad_close(ad);
      return;
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
	Add(phrase);
	return;
      } else {
	fprintf(stderr, "Phrase was null\n");
	ad_close(ad);
	return;
      }

      if (ps_start_utt(ps) < 0) {
	fprintf(stderr, "Failed to start utterance\n");
	ad_close(ad);
	return;
      }
    }
  } // while(1)
}

void CleanUp()
{
  ps_free(ps);
  cmd_ln_free_r(config);
}

// Takes phrase and adds first two numbers and prints out results
void Add(char const* phrase) {

  char* phrase_copy = malloc(sizeof(char) * strlen(phrase));
  strcpy(phrase_copy, phrase);
  
  int value_1 = -1;
  int value_2 = -1;
  // printf("What was said: %s\n", phrase);

  char* token = strtok(phrase_copy, " ");

  while ( token != NULL ) {

    if      (strcmp(token, "ZERO")  == 0) { value_1 = 0; break; }
    else if (strcmp(token, "ONE")   == 0) { value_1 = 1; break; }
    else if (strcmp(token, "TWO")   == 0) { value_1 = 2; break; }
    else if (strcmp(token, "THREE") == 0) { value_1 = 3; break; }
    else if (strcmp(token, "FOUR")  == 0) { value_1 = 4; break; }
    else if (strcmp(token, "FIVE")  == 0) { value_1 = 5; break; }
    else if (strcmp(token, "SIX")   == 0) { value_1 = 6; break; }
    else if (strcmp(token, "SEVEN") == 0) { value_1 = 7; break; }
    else if (strcmp(token, "EIGHT") == 0) { value_1 = 8; break; }
    else if (strcmp(token, "NINE")  == 0) { value_1 = 9; break; }	
    
    token = strtok(NULL, " ");
  }

  if (value_1 < 0) {
    printf("\nTHERE WERE NO NUMBERS IN PHRASE\n\n");
    free(phrase_copy);
    return;
  }

  // search for second number
  token = strtok(NULL, " ");
  
  while ( token != NULL ) {

    if      (strcmp(token, "ZERO")  == 0) { value_2 = 0; break; }
    else if (strcmp(token, "ONE")   == 0) { value_2 = 1; break; }
    else if (strcmp(token, "TWO")   == 0) { value_2 = 2; break; }
    else if (strcmp(token, "THREE") == 0) { value_2 = 3; break; }
    else if (strcmp(token, "FOUR")  == 0) { value_2 = 4; break; }
    else if (strcmp(token, "FIVE")  == 0) { value_2 = 5; break; }
    else if (strcmp(token, "SIX")   == 0) { value_2 = 6; break; }
    else if (strcmp(token, "SEVEN") == 0) { value_2 = 7; break; }
    else if (strcmp(token, "EIGHT") == 0) { value_2 = 8; break; }
    else if (strcmp(token, "NINE")  == 0) { value_2 = 9; break; }	
    
    token = strtok(NULL, " ");
  }

  free(phrase_copy);
  
  if (value_2 < 0) {
    printf("\nTHERE WAS ONLY ONE NUMBERS IN PHRASE\n\n");
    return;
  }

  // showcase answer
  printf("\n%d + %d = %d\n\n", value_1, value_2, value_1 + value_2);  
}

int main(int argc, char* argv[]) {
  
  DictionarySetup();

  GetPhrase();
  
  CleanUp();
}
