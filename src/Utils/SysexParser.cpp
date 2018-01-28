#include "SysexParser.h"


/**
 * C-style binary file parser into structs
 * This is definitely not the right way to do this
 * but probably the easiest
 */

// TODO : Refactor & make more robust
void SysexParser::parse(std::string filename) {
    std::ifstream file;
    file.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);

    struct common_data {
          char header[4];
        char byte_count[2];
        char address[3];
        char name[10];
        char reserved1[4];
        uint8_t category;
        uint8_t reserved2;
        uint8_t LFO1_waveform;
        uint8_t LFO1_speed;
        uint8_t LFO1_delay;
        uint8_t LFO1_key_sync;
        uint8_t reserved3;
        uint8_t LFO1_pitch_mod;
        uint8_t LFO1_amplitude_mod;
        uint8_t LFO1_freq_mod;
        uint8_t LFO2_waveform;
        uint8_t LFO2_speed;
        uint8_t reserved4[2];
        uint8_t LFO2_phase;
        uint8_t LFO2_key_sync;

        uint8_t note_shift;
        uint8_t pitch_eg_level0;
        uint8_t pitch_eg_level1;
        uint8_t pitch_eg_level2;
        uint8_t pitch_eg_level4;
        uint8_t pitch_eg_time1;
        uint8_t pitch_eg_time2;
        uint8_t pitch_eg_time3;
        uint8_t pitch_eg_time4;
        uint8_t pitch_eg_velocity;

        char fseq_voiced_switch[2];
        char fseq_unvoiced_switch[2];
        uint8_t algorithm;
        uint8_t voiced_correction[8];
        uint8_t reserved5[6];
        uint8_t pitch_eg_range;
        uint8_t pitch_eg_time_scaling;
        uint8_t pitch_eg_level3;

        uint8_t voiced_feedback;
        uint8_t reserved6;
        uint8_t formant_destination[5];
        uint8_t formant_depth[5];
        uint8_t fm_destination[5];
        uint8_t fm_depth[5];

        uint8_t filter_type;
        uint8_t filter_resonance;
        uint8_t filter_resonance_vel;
        uint8_t filter_cutoff;
        uint8_t filter_eg;
        uint8_t filter_freq_LFO1;
        uint8_t filter_freq_LFO2;
        uint8_t filter_cutoff_scale_depth;
        uint8_t filter_cutoff_scale_point;
        uint8_t filter_input_gain;
        uint8_t reserved7[6];
        
        uint8_t filter_depth;
        uint8_t filter_eg_level4;
        uint8_t filter_eg_level1;
        uint8_t filter_eg_level2;
        uint8_t filter_eg_level3;
        uint8_t filter_eg_time1;
        uint8_t filter_eg_time2;
        uint8_t filter_eg_time3;
        uint8_t filter_eg_time4;
        uint8_t reserved8;
        uint8_t filter_eg_attack;
        uint8_t reserved9;
    };

    struct voice_data {
        uint8_t osc_sync_transpose;
        uint8_t osc_freq_coarse;
        uint8_t osc_freq_fine;
        uint8_t osc_freq_scaling;
        uint8_t osc_bw_bias;
        uint8_t osc_bw_spectr_skirt;
        uint8_t osc_freq_ratio;
        uint8_t osc_freq_detune;
        uint8_t osc_freq_eg_init_val;
        uint8_t osc_freq_eg_attack_val;
        uint8_t osc_freq_eg_attack_time;
        uint8_t osc_freq_eg_decay_time;
        uint8_t eg_level1;
        uint8_t eg_level2;
        uint8_t eg_level3;
        uint8_t eg_level4;
        uint8_t eg_time1;
        uint8_t eg_time2;
        uint8_t eg_time3;
        uint8_t eg_time4;
        uint8_t eg_hold_time;
        uint8_t eg_time_scaling;
        uint8_t level_total;
        uint8_t level_breakpoint;
        uint8_t level_left_depth;
        uint8_t level_right_depth;
        uint8_t level_left_curve;
        uint8_t level_right_curve;
        uint8_t reserved1[3];
        uint8_t freq_bias;
        uint8_t freq_mod;
        uint8_t amp_mod_vel;
        uint8_t eg_bias;
    };

    struct data {
        common_data common;
        voice_data voice[8];
    } d;

    file.read((char*)(&d.common), sizeof(common_data));

    for (int i = 0; i < 8; i++) {
        file.read((char*)(&d.voice[i]), sizeof(voice_data));
    }
}
