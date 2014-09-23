#include <ruby.h>
#include <pocketsphinx.h>

#ifdef HAVE_FRAMEWORK_OPENAL
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

static const arg_t cont_args_def[] = {
    POCKETSPHINX_OPTIONS,
    /* Argument file. */
    {"-argfile",
     ARG_STRING,
     NULL,
     "Argument file giving extra arguments."},
    CMDLN_EMPTY_OPTION
};

typedef struct {
    ps_decoder_t * decoder;
} Transcriber;

static VALUE rb_mRist,
             rb_cTranscriber;

static void transcriber_deallocate(Transcriber * transcriber) {
    if (transcriber -> decoder)
        ps_free(transcriber -> decoder);
    xfree(transcriber);
}

static VALUE transcriber_allocate(VALUE self) {
    Transcriber * transcriber;
    return Data_Make_Struct(self, Transcriber, 0, transcriber_deallocate, transcriber);
}

static VALUE transcriber_initialize_pocketsphinx(VALUE self, VALUE arguments) {
    Transcriber * transcriber;
    char ** argv;
    cmd_ln_t * config;
    int i, argc;

    argc = RARRAY_LEN(arguments);
    argv = ALLOC_N(char *, argc);
    for (i = 0; i < argc; i++) {
        argv[i] = RSTRING_PTR(rb_ary_entry(arguments, i));
    }
    config = cmd_ln_parse_r(NULL, cont_args_def, argc, argv, TRUE);
    xfree(argv);

    if (config == NULL)
        rb_raise(rb_eArgError, "bad configuration");
    ps_default_search_args(config);

    Data_Get_Struct(self, Transcriber, transcriber);
    if ((transcriber -> decoder = ps_init(config)) == NULL) {
        cmd_ln_free_r(config);
        rb_raise(rb_eRuntimeError, "failed to initialize pocketsphinx");
    }

    return self;
}

static void close_raise(ALCdevice * device, VALUE error, const char * message) {
    alcCaptureCloseDevice(device);
    rb_raise(error, "%s\n", message);
}

static int32 al_read(ALCdevice * device, int16 * buffer, int32 max) {
    ALCint number;

    alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, sizeof(number), &number);
    if (number >= 0) {
        number = (number < max ? number : max);
        alcCaptureSamples(device, buffer, number);
    }

    return number;
}

/* Transcribe speech continuously.
 *
 * for block { |utterance| ... }
 * @yield [utterance] do whatever you want to do with the utterance transcribed
 *
 * @yieldparam [String] utterance the utterance transcribed
 */
static VALUE transcriber_transcribe(VALUE self)
{
    ALCdevice * device;
    ALCuint frequency;
    Transcriber * transcriber;
    cmd_ln_t *config;
    const char * hyp;
    const char * uttid;
    int16 adbuf[4096];
    int32 k;
    ps_decoder_t * ps;
    uint8 utt_started, in_speech;

    Data_Get_Struct(self, Transcriber, transcriber);
    ps = transcriber -> decoder;
    config = ps_get_config(ps);
    frequency = cmd_ln_float32_r(config, "-samprate");

    device = alcCaptureOpenDevice(NULL, frequency, AL_FORMAT_MONO16, frequency * 10);
    if (device == NULL)
        rb_raise(rb_eRuntimeError, "failed to open audio device");
    alcCaptureStart(device);

    if (ps_start_utt(ps, NULL) < 0)
        close_raise(device, rb_eRuntimeError, "failed to start utterance");
    utt_started = FALSE;

    /* Indicate listening for next utterance */
    for (;;) {
        if ((k = al_read(device, adbuf, 4096)) < 0)
            close_raise(device, rb_eRuntimeError, "failed to read audio");
        rb_funcall(self, rb_intern("sleep"), 1, rb_float_new(0.1));
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        in_speech = ps_get_in_speech(ps);
        if (in_speech && !utt_started) {
            utt_started = TRUE;
        }
        if (!in_speech && utt_started) {
            //speech -> silence transition, 
            //time to start new utterance
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL, &uttid);

            // yield result
            rb_yield(rb_str_new2(hyp));

            if (ps_start_utt(ps, NULL) < 0)
                close_raise(device, rb_eRuntimeError, "failed to start utterance");
            /* Indicate listening for next utterance */
            utt_started = FALSE;
        }
    }
    alcCaptureCloseDevice(device);
}

void Init_transcriber() {
    rb_mRist = rb_define_module("Rist");
    rb_cTranscriber = rb_define_class_under(rb_mRist, "Transcriber", rb_cObject);
    rb_define_alloc_func(rb_cTranscriber, transcriber_allocate);
    rb_define_private_method(rb_cTranscriber, "initialize_pocketsphinx", transcriber_initialize_pocketsphinx, 1);
    rb_define_method(rb_cTranscriber, "transcribe", transcriber_transcribe, 0);
}
