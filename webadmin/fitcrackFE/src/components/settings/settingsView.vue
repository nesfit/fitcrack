<!--
  * Author : see AUTHORS
  * Licence: MIT, see LICENSE
-->

<template>
  <v-container>
    <v-row>
      <v-col>
        <v-card flat>
          <v-card-title>
            <v-icon left>
              mdi-palette
            </v-icon>
            <span>Appearance</span>
          </v-card-title>
          <v-card-text>
            <v-btn-toggle
              v-model="appearance"
              mandatory
              color="primary"
            >
              <v-btn value="auto">
                <v-icon
                  left
                  small
                  class="neutral ml-0"
                >
                  mdi-creation
                </v-icon>
                <span>Automatic</span>
              </v-btn>
              <v-btn value="light">
                <v-icon
                  left
                  small
                  class="neutral ml-0"
                >
                  mdi-brightness-5
                </v-icon>
                <span>Light</span>
              </v-btn>
              <v-btn value="dark">
                <v-icon
                  left
                  small
                  class="neutral ml-0"
                >
                  mdi-brightness-2
                </v-icon>
                <span>Dark</span>
              </v-btn>
            </v-btn-toggle>
            <div class="mt-2 ml-1">
              {{ appearanceHint }}<span v-show="testmode">. Temporarily toggle with CTRL+SHIFT+L anywhere.</span>
            </div>
            <v-switch
                v-model="confirmpurge"
                mandatory
                label="Ask to confirm job purge"
                hint="If enabled, Fitcrack asks for confirmation after clicking on Purge button."
                persistent-hint
                class="mb-4"
            />
           </v-card-text>
        </v-card>
        <v-card flat class="mt-6">
          <v-card-title>
            <v-icon left>
              mdi-test-tube
            </v-icon>
            <span>Development</span>
          </v-card-title>
          <v-card-text>
            <v-switch
              v-model="testmode"
              label="Developer mode"
              hint="Enables useful utilities throughout the app when developing Fitcrack."
              persistent-hint
              class="mb-4"
            />
          </v-card-text>
        </v-card>
      </v-col>
      <!-- // system -->
      <v-col>
        <v-card
          flat
          min-width="300"
        >
          <v-card-title>
            <v-icon left>
              mdi-settings-box
            </v-icon>
            <span>System preferences</span>
          </v-card-title>
          <v-card-text>
            <v-text-field
              v-model="settings.default_seconds_per_workunit"
              :loading="loading"
              outlined
              type="number"
              label="Default time per workunit"
              :hint="wuTimeHint"
              :color="settings.default_seconds_per_workunit < wutthresh ? 'warning' : ''"
              persistent-hint
              suffix="seconds"
              class="mb-4"
            />
            <v-text-field
              v-model="settings.default_workunit_timeout_factor"
              :loading="loading"
              outlined
              type="number"
              step="0.1"
              label="Workunit timeout factor"
              hint="Multiplying factor for workunit timeout — the time after which a workunit is considered failed."
              persistent-hint
              class="mb-4"
            />
            <v-text-field
              v-model="settings.default_hwmon_temp_abort"
              :loading="loading"
              outlined
              type="number"
              label="Temperature threshold"
              hint="Abort cracking if temperature of the client's PC reaches this threshold"
              persistent-hint
              suffix="°C"
              class="mb-4"
            />
            <v-switch
              v-model="settings.default_bench_all"
              :loading="loading"
              outlined
              label="Run full benchmark on join"
              hint="When enabled, new hosts connected to the system will run a complete first-time benchmark."
              persistent-hint
              class="mb-4"
            />
            <v-text-field
              v-model="settings.default_distribution_coefficient_alpha"
              :loading="loading"
              outlined
              type="number"
              label="Distribution coefficient α"
              hint="Maximum percentage of the remaining keyspace that can be assigned with a single workunit unless it would be below the minimum."
              persistent-hint
              class="mb-4"
            />
            <v-text-field
              v-model="settings.default_t_pmin"
              :loading="loading"
              outlined
              type="number"
              label="T_pmin"
              min="10"
              max="3600"
              hint="Absolute minimum seconds per workunit (including the start of hashcat, etc.). Prevents creation of extremely small workunits."
              persistent-hint
              suffix="seconds"
              class="mb-4"
            />
            <v-switch
              v-model="settings.default_ramp_up_workunits"
              :loading="loading"
              outlined
              label="Ramp-up workunits"
              hint="If enabled, Fitcrack creates smaller workunits at start. The size increases until the solving time hits the “Time per workunit” value."
              persistent-hint
              class="mb-4"
            />
            <v-text-field
              v-model="settings.default_ramp_down_coefficient"
              :loading="loading"
              outlined
              type="number"
              min="0.0"
              max="1.0"
              label="Ramp down coefficient"
              hint="Minimum fraction of “Time per workunit” that can be created. Influences the size of workunits at the end of the job. The lower the value, the smaller the size workunits at the end. 1.0 means no ramp down, 0.0 ramp down is limited only by T_pmin."
              persistent-hint
              class="mb-4"
            />
          </v-card-text>
          <v-card-actions>
            <v-spacer />
            <v-btn
              color="primary"
              :loading="saving"
              @click="saveSettings"
            >
              <v-icon left>
                mdi-content-save
              </v-icon>
              Save
            </v-btn>
          </v-card-actions>
        </v-card>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
    export default {
      name: "Settings",
      data: function() {
      return {
          appearance: localStorage.getItem('appearance') || 'auto',
          testmode: localStorage.getItem('testmode') == 'true' || false,
          settings: {},
          loading: true,
          saving: false,
          wutthresh: 180, // minimum reccomended seconds per WU
          confirmpurge: localStorage.getItem('confirmpurge') == 'true' || false
        }
      },
      computed: {
        appearanceHint () {
          switch (this.appearance) {
            case 'auto':
              return 'Webadmin will change appearance dynamically based on your system theme'
            case 'light':
              return 'Webadmin will retain light appearance regardless of your system theme'
            case 'dark':
              return 'Webadmin will retain dark appearance regardless of your system theme'
            default:
              return undefined
          }
        },
        wuTimeHint () {
          if (this.settings.default_seconds_per_workunit && this.settings.default_seconds_per_workunit < this.wutthresh) {
            return 'Setting extremely low time per workunit leads to high overhead. Cracking may take much longer than estimated. Consider choosing a higher default value.'
          } else {
            return 'The time per workunit preference used for new jobs. You can change it on a per-job basis in the additional settings step when creating a job or via the edit job dialog.'
          }
        }
      },
      watch: {
        appearance (value) {
          localStorage.setItem('appearance', value)
          if (value == 'dark') this.$vuetify.theme.dark = true
          if (value == 'light') this.$vuetify.theme.dark = false
          if (value == 'auto') this.$vuetify.theme.dark = window.matchMedia('(prefers-color-scheme: dark)').matches
        },
        testmode (value) {
          localStorage.setItem('testmode', value)
        },
        confirmpurge (value) {
          localStorage.setItem('confirmpurge', value)
        }
      },
      mounted () {
        this.loadSettings()
      },
      methods: {
        async loadSettings () {
          this.loading = true
          this.settings = await this.axios.get(this.$serverAddr + '/settings').then(r => r.data)
          this.loading = false
        },
        saveSettings () {
          this.saving = true
          this.axios.post(this.$serverAddr + '/settings', {
            ...this.settings
          })
          .then(() => {
            this.saving = false
            this.$store.state.jobForm.timeForJob = this.settings.default_seconds_per_workunit
          })
        }
      }
  }
</script>



<style scoped>
.neutral {
  color: unset !important
}
</style>
