<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="containerAddJob height100 mx-auto">
    <v-scale-transition>
      <v-sheet
        v-show="keyspaceKnown && keyspace || estimatedTime"
        class="infobar"
        color="info"
        dark
        elevation="8"
      >
        <div class="d-flex">
          <div
            v-show="keyspace"
            class="text-end"
          >
            <div class="overline">
              <v-icon
                x-small
                class="mr-1"
              >
                mdi-key
              </v-icon>
              <span>
                Keyspace
              </span>
            </div>
            <div>{{ numberFormat(keyspace) }}</div>
          </div>
          <div
            v-show="estimatedTime"
            class="ml-4 text-end"
          >
            <div class="overline">
              <v-icon
                x-small
                class="mr-1"
              >
                mdi-timelapse
              </v-icon>
              <span>
                Est. cracking time
              </span>
            </div>
            <div>{{ estimatedTime }}</div>
          </div>
        </div>
      </v-sheet>
    </v-scale-transition>
    <div
      v-show="loading"
      class="loadingProgressCont"
    >
      <v-progress-circular
        indeterminate
        color="primary"
        size="65"
        class="loadingProgress"
        :width="4"
      />
    </div>

    <v-row>
      <v-col>
        <v-alert
          v-if="!helpAlreadyDismissed"
          text
          type="success"
          icon="mdi-help-box"
          dismissible
        >
          If you need help with creating a job, you can <a target="_blank" :href="$docsLink + '/#/jobs/creating/overview'">read the user manual here</a>.
          <template #close="{ toggle }">
            <v-btn
              text
              small
              @click="dismissHelp(toggle)"
            >
              Don't show anymore
              <v-icon right>mdi-close</v-icon>
            </v-btn>
          </template>
        </v-alert>
        <v-alert
          v-if="helpDismissedMessage"
          text
          type="info"
          dismissible
        >
          Sure. If you need help at any time in the future, use the link in the top right corner.
        </v-alert>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <v-alert
          v-if="slowCandidatesMessage"
          text
          type="info"
          dismissible
        >
        You have selected a slow hash type. Slow candidates mode has been automatically activated for you.
        </v-alert>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <v-text-field
          v-model="name"
          outlined
          autofocus
          required
          label="Name"
          hint="Give this job a descriptive name"
          persistent-hint
        />
      </v-col>
      <v-col>
        <v-autocomplete
          :items="templates"
          item-text="name"
          item-value="id"
          :value="selectedTemplate"
          auto-select-first
          outlined
          label="Template"
          hint="Prefill the form with a saved template (use empty to quickly reset)"
          persistent-hint
          @input="fetchAndApplyTemplate"
        />
      </v-col>
    </v-row>

    <div>
      <v-col>
        <v-stepper
          id="job-stepper"
          v-model="step"
          vertical
          non-linear
          class="mb-4"
        >
          <v-stepper-step
            id="job-step-1"
            editable
            step="1"
            :color="hashListId ? 'success' : 'primary'"
            :complete="!!hashListId"
            edit-icon="mdi-check"
          >
            {{ hashlistStepTitle }}
          </v-stepper-step>
          <v-stepper-content step="1">
            <v-container>
              <v-row>
                <v-col>
                  <v-btn
                    large block
                    color="primary"
                    :to="{name: 'createHashlist', query: {attach: `${name} Hashlist`}}"
                  >
                    <v-icon left>mdi-book-plus</v-icon>
                    Create new input hashlist
                  </v-btn>
                </v-col>
              </v-row>
              <v-row>
                <v-col>
                  <div class="text-center text-overline text--secondary">or attach an existing one</div>
                  <HashlistSelector
                  v-model="hashListId"
                  hide-completed
                  />
                </v-col>
              </v-row>
              <v-row class="mb-2">
                <v-spacer></v-spacer>
                <v-btn
                  color="primary"
                  @click="step = 2"
                  :disabled="!hashListId"
                >
                  {{ hashListId ? 'Next' : 'Select a hashlist above' }}
                </v-btn>
              </v-row>
            </v-container>
          </v-stepper-content>

          <v-stepper-step
            id="job-step-2"
            editable
            step="2"
          >
            Attack settings
          </v-stepper-step>
          <v-stepper-content step="2">
            <v-container>
              <v-card-title>Attack mode</v-card-title>
              <v-item-group
                v-model="attackSettingsTab"
                class="d-flex flex-wrap justify-space-between"
              >
                <v-item
                  v-for="(type, i) in attacks"
                  :key="i"
                  :value="type.handler"
                  #default="{ active, toggle }"
                >
                  <v-btn
                    :id="`attack-mode-${type.handler}`"
                    class="mode-btn"
                    text
                    small
                    :color="active ? 'primary' : ''"
                    @click="toggle"
                  >
                    <div class="my-3">
                      <v-icon class="mb-2">
                        {{ attackIcon(type.handler) }}
                      </v-icon>
                      <div>{{ type.name }}</div>
                    </div>
                  </v-btn>
                </v-item>
              </v-item-group>

              <v-component :id="attackSettingsTab" :is="attackSettingsTab" :keyspace="keyspace" />

              <v-row>
                <v-spacer />
                <v-btn
                  class="mr-6 mt-4"
                  color="primary"
                  @click="step = 3"
                >
                  Next
                </v-btn>
              </v-row>
            </v-container>
          </v-stepper-content>
          <v-stepper-step
            id="job-step-3"
            editable
            step="3"
          >
            Host assignment
          </v-stepper-step>
          <v-stepper-content step="3">
            <v-container>
              <v-subheader>Select which hosts to distribute workunits to</v-subheader>
              <div class="scroller">
                <host-selector
                  v-model="hosts"
                  select-all
                  auto-refresh
                />
              </div>
              <v-subheader><b>({{ hosts.length }} selected)</b></v-subheader>
              <v-row>
                <v-spacer />
                <v-btn
                  class="mr-6 mt-4"
                  color="primary"
                  @click="step = 4"
                >
                  Next
                </v-btn>
              </v-row>
            </v-container>
          </v-stepper-content>
          <v-stepper-step
            id="job-step-4"
            editable
            step="4"
          >
            Additional settings
          </v-stepper-step>
          <v-stepper-content step="4">
            <v-container>
              <v-row>
                <v-textarea
                  id="comment-input"
                  v-model="comment"
                  label="Comment"
                  outlined
                  auto-grow
                  :rows="1"
                />
              </v-row>
              <v-row>
                <v-col>
                  <div class="title mb-2">Planned start</div>
                  <dt-picker
                    v-model="startDate"
                    outlined
                    hide-details
                    :disabled="startNow"
                    single-line
                    label=""
                  />
                  <v-checkbox
                    v-model="startNow"
                    label="Immediately"
                  />
                </v-col>
                <v-col>
                  <div class="title mb-2">
                    Desired time per workunit
                  </div>
                  <v-text-field
                    v-model="timeForJob"
                    outlined
                    hide-details
                    single-line
                    label="Time for job"
                    required
                    type="number"
                    suffix="seconds"
                    :min="60"
                  />
                </v-col>
                <v-col>
                  <div class="title mb-2">Planned end</div>
                  <dt-picker
                    v-model="endDate"
                    outlined
                    hide-details
                    :disabled="endNever"
                    single-line
                  />
                  <v-checkbox
                    v-model="endNever"
                    label="No time limit"
                  />
                </v-col>
              </v-row>
            </v-container>
          </v-stepper-content>
        </v-stepper>

        <v-row
          v-if="hosts.length == 0"
          justify="center"
        >
          <v-alert
            outlined
            type="warning"
          >
            This job has no hosts assigned to it and won't be able to run until edited!
          </v-alert>
        </v-row>

        <v-row
          v-if="timeForJob < 180"
          justify="center"
        >
          <v-alert
            outlined
            type="warning"
          >
            Setting extremely low time per workunit leads to high overhead. The cracking may take much longer than estimated. Consider choosing a higher value.
          </v-alert>
        </v-row>

        <v-row
          justify="center"
          class="mb-5 mt-2"
        >
          <template-modal
            :inherited-name="selectedTemplateName"
            @templatesUpdated="fetchTemplates"
          />
          <v-btn
            large
            color="primary"
            class="ml-2"
            @click="submit"
          >
            <v-icon left>
              {{ canCreateJob() ? 'mdi-check' : 'mdi-content-save' }}
            </v-icon>
            {{ canCreateJob() ? 'Create' : 'Save for later' }}
          </v-btn>
        </v-row>
      </v-col>
    </div>
  </div>
</template>

<script>
  import sha1 from 'sha1'
  import numberFormat from '@/assets/scripts/numberFormat'
  import { attackIcon } from '@/assets/scripts/iconMaps'

  import combinator from '@/components/job/attacks/combinator.vue'
  import mask from '@/components/job/attacks/mask.vue'
  import dictionary from '@/components/job/attacks/dictionary.vue'
  import hybridMaskWordlist from '@/components/job/attacks/hybridMaskWordlist.vue'
  import hybridWordlistMask from '@/components/job/attacks/hybridWordlistMask.vue'
  import pcfgAttack from '@/components/job/attacks/pcfg.vue'
  import princeAttack from '@/components/job/attacks/prince.vue'
  import FileUploader from "@/components/fileUploader/fileUploader.vue";
  import fcTextarea from '@/components/textarea/fc_textarea.vue'
  import hostSelector from '@/components/selector/hostSelector.vue'
  import templateModal from '@/components/jobTemplate/templateModal.vue'
  import dtPicker from '@/components/picker/datetime.vue'

  import {mapState, mapGetters, mapMutations} from 'vuex'
  import {mapTwoWayState} from 'spyfu-vuex-helpers'
  import {twoWayMap} from '@/store'

  import { attacks } from '@/store/job-form'
  import HashlistCreator from '@/components/hashlist/hashlistCreator.vue'
  import HashlistSelector from '@/components/selector/hashlistSelector.vue'

  export default {
    name: 'AddJob',
    components: {
    FileUploader,
    'combinator': combinator,
    'maskattack': mask,
    'dictionary': dictionary,
    'hybridMaskWordlist': hybridMaskWordlist,
    'hybridWordlistMask': hybridWordlistMask,
    'pcfgAttack': pcfgAttack,
    'princeAttack': princeAttack,
    'fc-textarea': fcTextarea,
    'host-selector': hostSelector,
    'template-modal': templateModal,
    dtPicker,
    HashlistCreator,
    HashlistSelector
},
    data: function () {
      return {
        loading: false,
        helpDismissedMessage: false,
        hashTypes: [],
        slowCandidatesMessage: false,
        showEstimatedTime: false,
        estimatedTime: null,
        keyspace: null,
        selectedTemplateName: '',
        attacks,
        templates: [
          {
            name: 'Empty',
            id: 0
          }
        ]
      }
    },
    computed: {
      ...mapState('jobForm', ['selectedTemplate']),
      ...mapTwoWayState('jobForm', twoWayMap([
        'step', 'attackSettingsTab', 'hashListId', 'name', 'startDate', 'endDate', 'template', 'comment', 'hosts', 'startNow', 'endNever', 'timeForJob', 'slowCandidates'
      ])),
      ...mapGetters('jobForm', ['jobSettings', 'valid', 'validAttackSpecificSettings', 'keyspaceKnown']),
      templateItems () {
        return this.templates.map((t, i) => ({text: t.template, value: i}))
      },
      dev () {
        return localStorage.getItem('testmode') == 'true'
      },
      helpAlreadyDismissed () {
        return localStorage.getItem('dismissedHelp') == 'true'
      },
      hashlistStepTitle () {
        return this.hashListId ? 'Hashlist attached' : 'Hashlist selection'
      }
    },
    watch: {
      jobSettings (val) {
        // Reset old values as they are no longer valid
        this.estimatedTime = null
        this.keyspace = null
        if (val.attack_settings != false && this.validAttackSpecificSettings) {
          var boincIds = []
          for (let i = 0; i < this.hosts.length; i++) {
            boincIds.push(this.hosts[i].id)
          }
          // Compute new keyspace and new estimation of cracking time
          if (!this.hashListId) return // early return if no hashlist is selected
          this.axios.post(this.$serverAddr + '/job/crackingTime', {   
            'hash_list_id': this.hashListId,
            'boinc_host_ids': boincIds.join(","),
            'attack_settings': JSON.stringify(val.attack_settings),
            'workunit_time': this.timeForJob
          }).then((response) => {
            if (response['data']) {
              this.estimatedTime = response.data.display_time
              this.keyspace = response.data.keyspace

              // Enable slow candidates if slow hash is selected
              if(this.slowCandidatesMessage == false && (val.attack_settings)['attack_mode'] == 0 && (val.attack_settings).rules)
              {
                var hashType = this.hashTypes.find(h => h.code == response.data.hash_code)
                if(hashType != null && hashType.isSlow)
                {
                  this.$infoMessage('Slow hash detected; slow candidates mode has been turned on.')
                  this.slowCandidatesMessage = true
                  this.slowCandidates = true
                }
              }
            }
          })
        }
      }
    },
    mounted: function () {
      this.loadSettings()
      this.startDate = this.$moment().format('YYYY-MM-DDTHH:mm')
      this.endDate = this.$moment().format('YYYY-MM-DDTHH:mm')
      this.fetchTemplates()
      if (this.name === '') {
        this.generateJobName()
      }
      this.getHashTypes()
    },
    methods: {
      ...mapMutations('jobForm', ['applyTemplate']),
      numberFormat,
      attackIcon,
      async loadSettings () {
        if (!this.timeForJob) {
          const settings = await this.axios.get(this.$serverAddr + '/settings').then(r => r.data)
          this.timeForJob = settings.default_seconds_per_workunit
        }
      },
      dismissHelp (toggleFunction) {
        localStorage.setItem('dismissedHelp', true)
        toggleFunction()
        this.helpDismissedMessage = true
      },
      fetchTemplates () {
        this.axios.get(this.$serverAddr + '/template')
        .then((response) => {
          if (response.data && response.data.items) {
            this.templates = [
              { name: 'Empty', id: 0 },
              ...response.data.items
            ]
          }
        })
        .catch(console.error)
      },
      fetchAndApplyTemplate (id) {
        if (id == 0) {
          this.applyTemplate()
          this.selectedTemplateName = ''
          this.$store.commit('jobForm/selectedTemplateMut', 0)
          this.loadSettings()
          return
        }
        this.axios.get(this.$serverAddr + `/template/${id}`)
        .then((response) => {
          if (response.data && response.data.template) {
            const data = JSON.parse(response.data.template)
            this.applyTemplate(data)
            this.selectedTemplateName = data.template
            this.$store.commit('jobForm/selectedTemplateMut', id)
          }
        })
        .catch(console.error)
      },
      getHashTypes () {
        this.axios.get(this.$serverAddr + '/hashcat/hashTypes').then((response) => {
          this.hashTypes = response.data.hashtypes
        })
      },
      submit () {
        // TODO: maybe delete this condition
        if (this.name === '') {
          this.$error('Job name can not be empty.')
          return
        }

        if (!this.attackSettingsTab) {
          this.$error('No attack selected.')
          this.step = 2
          return
        }

        if (this.jobSettings.attack_settings === false) {
          this.$error('Error in attack settings.')
          this.step = 2
          return
        }

        if (this.keyspace > 1.8446744e+19 /* 2^64 */) {
          this.$error('Job keyspace is higher than maximal allowed value 2^64.')
          this.step = 2
          return
        }

        if (this.timeForJob < 10) {
          this.$error('Time per workunit must be higher or equal to 10 seconds.')
          this.step = 4
          return
        }

        // Check if all job settings are valid
        if (!this.valid) {
          // If all checks above passed and this one did not, it means that attack specific settings are incorrect.
          this.$error('Error in attack settings.')
          this.step = 2
          return
        }

        this.loading = true
        const finalStartTime = this.startNow ? 
          this.jobSettings['time_start'] : 
          this.$moment(this.jobSettings['time_start']).utc().toISOString(true).slice(0, 16)
        const finalEndTime = this.endNever ? 
          this.jobSettings['time_end'] : 
          this.$moment(this.jobSettings['time_end']).utc().toISOString(true).slice(0, 16)
        const finalSettings = {
          ...this.jobSettings,
          'time_start': finalStartTime,
          'time_end': finalEndTime
        }
        this.axios.post(this.$serverAddr + '/job', finalSettings).then((response) => {
          this.$router.push({name: 'jobDetail', params: {id: response.data.job_id}})
          this.applyTemplate() // Clear all
        }).catch((error) => {
          this.loading = false
        })
      },
      getRandomHash () {
        const randomPass = Math.random().toString(36).substring(2,6)
        this.hashList += `${sha1(randomPass)}\n`
        this.hashType = { code: '100', name: 'SHA1' },
        this.comment += `(${randomPass}) `
        this.validateHashes()
      },
      generateJobName () {
        this.name = this.$store.state.project + ' Job – ' + this.$moment().format('DD.MM.YYYY HH:mm')
      },
      canCreateJob() {
        return this.hosts.some((host) => host.last_active.online);
      }
    }
  }
</script>

<style scoped>
  .containerAddJob {
    padding: 2em;
    padding-top: 54px;
    position: relative;
    max-width: 1300px;
  }

  .addJobContent {
    width: 100%;
  }

  .max500 {
    max-width: 500px;
    width: 100%;
  }

  .max800 {
    max-width: 800px;
    width: 100%;
  }

  .max1000 {
    max-width: 1000px;
  }

  .infobar {
    position: fixed;
    z-index: 5;
    bottom: 1.2em;
    right: 1.2em;
    padding: 0.5em 1.5em;
    border-radius: 2em;
  }

  .filetype-link {
    color: inherit
  }

  .mode-btn {
    height: initial !important;
    margin: 1em;
  }

  .scroller {
    max-height: 400px;
    overflow-y: auto;
  }
</style>
