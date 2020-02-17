<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="containerAddJob height100 mx-auto">
    <v-toolbar
      v-show="estimatedTime || keyspace"
      class="infobar"
      floating
      dense
      :tile="false"
    >
      <div>
        <span v-show="keyspace" class="mr-1">
          Keyspace: {{ keyspace }}.
        </span>
        <span v-show="estimatedTime">
          Estimated cracking time is {{ estimatedTime }}.
        </span>
      </div>
    </v-toolbar>
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
          hint="Prefill the form with a saved template"
          persistent-hint
          @change="fetchAndApplyTemplate"
        />
      </v-col>
    </v-row>

    <div>
      <v-col>
        <v-stepper
          v-model="step"
          vertical
          non-linear
          class="mb-4"
        >
          <v-stepper-step 
            editable
            step="1"
          >
            Input settings
          </v-stepper-step>
          <v-stepper-content step="1">
            <v-container>
              <v-row class="mb-4">
                <v-btn-toggle
                  v-model="inputMethod"
                  mandatory
                  color="primary"
                  class="mr-2"
                >
                  <v-btn value="multipleHashes">
                    Manual entry
                  </v-btn>
                  <v-btn value="hashFile">
                    From hash file
                  </v-btn>
                  <v-btn value="extractFromFile">
                    Extract from file
                  </v-btn>
                </v-btn-toggle>
                <v-autocomplete
                  v-model="hashType"
                  editable
                  validate-on-blur
                  clearable
                  label="Select hash type"
                  :items="hashTypes"
                  item-text="name"
                  return-object
                  required
                  hide-details
                  single-line
                  flat
                  solo-inverted
                  @change="validateHashes(null)"
                >
                  <template
                    slot="item"
                    slot-scope="data"
                  >
                    <v-list-item-content>
                      <v-list-item-title><b>{{ data.item.code }}</b> - {{ data.item.name }}</v-list-item-title>
                    </v-list-item-content>
                  </template>
                </v-autocomplete>
              </v-row>
              <v-row>
                <v-col
                  v-if="inputMethod === 'extractFromFile'"
                  cols="12"
                >
                  <v-alert
                    type="warning"
                  >
                    Currently we support these formats:
                    <v-tooltip top>
                      <template v-slot:activator="{ on }">
                        <span v-on="on"><a href="#">MS_OFFICE</a>,</span>
                      </template>
                      <span>Hashtypes: 9400, 9500, 9600, 9700, 9800</span>
                    </v-tooltip>
                    <v-tooltip top>
                      <template v-slot:activator="{ on }">
                        <span v-on="on"><a href="#">PDF</a>,</span>
                      </template>
                      <span>Hashtypes: 10400, 10500, 10600, 10700</span>
                    </v-tooltip>
                    <v-tooltip top>
                      <template v-slot:activator="{ on }">
                        <span v-on="on"><a href="#">RAR</a> and </span>
                      </template>
                      <span>Hashtypes: 12500, 13000</span>
                    </v-tooltip>
                    <v-tooltip top>
                      <template v-slot:activator="{ on }">
                        <span v-on="on"><a href="#">ZIP</a>.</span>
                      </template>
                      <span>Hashtypes: 13600</span>
                    </v-tooltip>
                  </v-alert>
                  <file-uploader
                    ref="encryptedFileUploader"
                    :url="this.$serverAddr + '/protectedFiles/add'"
                    @uploadComplete="uploadComplete"
                  />
                </v-col>
                <v-col
                  v-if="inputMethod === 'hashFile'"
                  cols="12"
                >
                  <v-alert
                    type="warning"
                    class="mt-0 mb-0"
                  >
                    Input to hashcat. Can be binary hash (WPA/WPA2), or hashlist.
                  </v-alert>
                  <file-uploader
                    ref="hashFileUploader"
                    :no-upload="true"
                    @filesChanged="hashFileSelected"
                  />
                </v-col>
                <v-col cols="12">
                  <v-alert
                    :value="gotBinaryHash"
                    text
                    type="info"
                    color="primary"
                    class="mt-0 mb-0"
                  >
                    You can select only one binary hash.
                  </v-alert>
                  <fc-textarea
                    v-if="inputMethod !== null"
                    ref="textarea"
                    v-model="hashList"
                    :class="{error: hashListError}"
                    class="textarea"
                    max-height="500"
                    :readonly="!(inputMethod === 'multipleHashes' && !gotBinaryHash) "
                    :can-remove-line="true "
                    @blur="validateHashes"
                    @focus="unvalidateHashes"
                  >
                    <div
                      slot="after"
                      class="hashCeckContainer pl-1 pt-2"
                    >
                      <div
                        v-for="hashObj in validatedHashes"
                        :key="hashObj.id"
                      >
                        <v-icon
                          v-if="hashObj.result === 'OK'"
                          small
                          color="success"
                        >
                          check_circle_outlined
                        </v-icon>
                        <v-tooltip
                          v-else
                          left
                        >
                          <template v-slot:activator="{ on }">
                            <v-icon
                              small
                              color="error"
                              class="clickable"
                              v-on="on"
                            >
                              error_circle_outlined
                            </v-icon>
                          </template>
                          <span>{{ hashObj.result }}</span>
                        </v-tooltip>
                        <v-tooltip
                          v-if="hashObj.isInCache"
                          left
                        >
                          <template v-slot:activator="{ on }">
                            <v-icon
                              small
                              color="warning"
                              class="clickable"
                              v-on="on"
                            >
                              error_circle_outlined
                            </v-icon>
                          </template>
                          <span>hash already in hashcache</span>
                        </v-tooltip>
                      </div>
                    </div>
                  </fc-textarea>
                </v-col>
              </v-row>
              <v-row>
                <v-checkbox
                  v-show="invalidHashes.length > 0"
                  v-model="ignoreHashes"
                  label="Ignore invalid hashes"
                  hide-details
                  :height="15"
                  color="error"
                  class="ml-2 mt-1"
                />
                <v-spacer />
                <v-btn
                  color="primary"
                  @click="step = 2" 
                >
                  Next
                </v-btn>
              </v-row>
            </v-container>
          </v-stepper-content>

          <v-stepper-step 
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

              <v-component :is="attackSettingsTab" />

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
            editable
            step="3"
          >
            Host assignment
          </v-stepper-step>
          <v-stepper-content step="3">
            <v-subheader>Select which hosts to distribute workunits to</v-subheader>
            <host-selector
              v-model="hosts"
              :select-all="true"
            />
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
          </v-stepper-content>
          <v-stepper-step 
            editable
            step="4"
          >
            Additional settings
          </v-stepper-step>
          <v-stepper-content step="4">
            <v-container>
              <v-row>
                <v-textarea
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
                  <v-text-field
                    v-model="startDate"
                    outlined
                    hide-details
                    :disabled="startNow"
                    single-line
                    label=""
                    mask="date-with-time"
                  />
                  <v-checkbox
                    v-model="startNow"
                    label="Immediately"
                  />
                </v-col>
                <v-col>
                  <div class="title mb-2">Desired time for each job</div>
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
                  <v-text-field
                    v-model="endDate"
                    outlined
                    hide-details
                    :disabled="endNever"
                    single-line
                    label=""
                    mask="date-with-time"
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
          justify="center"
          class="mb-5"
        >
          <template-modal
            :inherited-name="name"
            @templatesUpdated="fetchTemplates"
          />
          <v-btn
            large
            color="primary"
            class="ml-2"
            :disabled="!valid || (invalidHashes.length > 0 && !ignoreHashes)"
            @click="submit"
          >
            <v-icon left>
              {{ hosts.length > 0 ? 'mdi-check' : 'mdi-content-save' }}
            </v-icon>
            {{ hosts.length > 0 ? 'Create' : 'Save for later' }}
          </v-btn>
        </v-row>
      </v-col>
    </div>
  </div>
</template>

<script>
  import combinator from '@/components/job/attacks/combinator'
  import mask from '@/components/job/attacks/mask'
  import dictionary from '@/components/job/attacks/dictionary'
  import hybridMaskWordlist from '@/components/job/attacks/hybridMaskWordlist'
  import hybridWordlistMask from '@/components/job/attacks/hybridWordlistMask'
  import pcfgAttack from '@/components/job/attacks/pcfg'
  import FileUploader from "@/components/fileUploader/fileUploader";
  import fcTextarea from '@/components/textarea/fc_textarea'
  import hostSelector from '@/components/selector/hostSelector'
  import templateModal from '@/components/jobTemplate/templateModal'

  import {mapState, mapGetters, mapMutations} from 'vuex'
  import {mapTwoWayState} from 'spyfu-vuex-helpers'
  import {twoWayMap} from '@/store'

  import { attacks } from '@/store/job-form'

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
      'fc-textarea': fcTextarea,
      'host-selector': hostSelector,
      'template-modal': templateModal
    },
    data: function () {
      return {
        loading: false,
        hashTypes: [],
        showEstimatedTime: false,
        estimatedTime: null,
        keyspace: null,
        gotBinaryHash: false,
        hashListError: false,
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
        'step', 'attackSettingsTab', 'validatedHashes', 'name', 'inputMethod', 'hashList', 'hashType', 'ignoreHashes', 'startDate', 'endDate', 'template', 'comment', 'hosts', 'startNow', 'endNever', 'timeForJob'
      ])),
      ...mapGetters('jobForm', ['jobSettings', 'valid']),
      templateItems () {
        return this.templates.map((t, i) => ({text: t.template, value: i}))
      },
      invalidHashes () {
        return this.validatedHashes.filter(h => h.result !== 'OK')
      }
    },
    watch: {
      jobSettings (val) {
        if ( typeof val.attack_settings !== 'object' || this.hashType == null || this.hosts.length === 0) {
          this.showEstimatedTime = false
        } else {
          var boincIds = []
          for (let i = 0; i < this.hosts.length; i++) {
            boincIds.push(this.hosts[i].id)
          }

          this.axios.get(this.$serverAddr + '/job/crackingTime', {
            params: {
              'hash_type_code': this.hashType,
              'boinc_host_ids': boincIds.join(","),
              'attack_settings': val.attack_settings
            }
          }).then((response) => {
            if (response['data']) {
              this.estimatedTime = response.data.display_time
              this.keyspace = response.data.keyspace
              this.showEstimatedTime = true
            }
          })
        }
      }
    },
    mounted: function () {
      this.getHashTypes()
      this.startDate = this.$moment().format('DD/MM/YYYY HH:mm')
      this.endDate = this.$moment().format('DD/MM/YYYY HH:mm')
      if (this.hashList.length > 0) this.validateHashes()
      this.fetchTemplates()
    },
    methods: {
      ...mapMutations('jobForm', ['applyTemplate']),
      fetchTemplates () {
        this.axios.get(this.$serverAddr + '/template', )
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
          this.$store.commit('jobForm/selectedTemplateMut', 0)
          return
        }
        this.axios.get(this.$serverAddr + `/template/${id}`)
        .then((response) => {
          console.log(response)
          if (response.data && response.data.template) {
            this.applyTemplate(JSON.parse(response.data.template))
            this.$store.commit('jobForm/selectedTemplateMut', id)
          }
        })
        .catch(console.error)
      },
      attackIcon (handler) {
        const map = {
          'dictionary': 'mdi-dictionary',
          'combinator': 'mdi-vector-combine',
          'maskattack': 'mdi-karate',
          'hybridWordlistMask': 'mdi-vector-difference-ba',
          'hybridMaskWordlist': 'mdi-vector-difference-ab',
          'pcfgAttack': 'mdi-ray-start-end'
        }
        return map[handler] || 'mdi-checkbox-blank-outline'
      },
      subHashtypeChanged: function (key, val) {
        this.hashType.code = this.hashType.code.replace(key, val.code)
        this.validateHashes(null)
      },
      focusTextarea: function () {
        this.$refs.textarea.focus()
      },
      validateHashes: function (data = null) {
        if (data === null) {
          data = this.hashList
        }
        var hashesList = data.split('\n')
        if (data.startsWith("BASE64:")) {
          this.gotBinaryHash = true
        } else {
          this.gotBinaryHash = false
        }
        if (this.hashType === null || isNaN(this.hashType.code)) {
          return
        }
        if (data === '') {
          return
        }
        ///
        const hashType = parseInt(this.hashType.code)
        if (hashType >= 17300 && hashType <= 18100) {
          return // Until validator can deal with Hashcat 5 modes
        }
        ///

        this.axios.post(this.$serverAddr + '/job/verifyHash', {
          'hashtype': this.hashType.code,
          'hashes': data
        }).then((response) => {
          this.hashListError = response.data.error
          this.validatedHashes = response.data.items
        })
      },
      unvalidateHashes: function (data) {
        this.validatedHashes = []
      },
      getHashTypes: function () {
        this.axios.get(this.$serverAddr + '/hashcat/hashTypes').then((response) => {
          this.hashTypes = response.data.hashtypes
        })
      },
      addHash: function (hash) {
        var parsedHashlist = this.hashList.split('\n')
        var lastHash = parsedHashlist[parsedHashlist.length-1]
        if (lastHash === '') {
          this.hashList = this.hashList + hash
        } else {
          this.hashList = this.hashList + '\n' + hash
        }
      },
      uploadComplete: function (data) {
        this.$success("Successfully extracted hash form file.")
        this.hashType = this.hashTypes.find(h => h.code == data['hash_type'])
        this.addHash(data['hash'])
        this.validateHashes(null)
      },
      isBinaryFile: function(content) {
        for (var i = 0; i < 24; i++) {
          var charCode = content.charCodeAt(i);
          if (charCode === 65533 || charCode <= 8) {
            return true
          }
        }
        return false
      },
      hashFileSelected: function (files) {
        var reader = new FileReader()
        reader.onloadend = function(evt) {
          if (evt.target.readyState == FileReader.DONE) {

            if (this.isBinaryFile(evt.target.result)) {
              // we got binary hash
              var binReader = new FileReader()
              binReader.onloadend = function(evt) {
                if (evt.target.readyState == FileReader.DONE) {
                  this.hashList = 'BASE64:' + evt.target.result.substr(evt.target.result.indexOf(',') + 1)
                  this.validateHashes(null)
                  this.gotBinaryHash = true
                }
              }.bind(this)
              binReader.readAsDataURL(files[0])

            } else {
              // we got hashlists
              var parsedHashlist = this.hashList.split('\n')
              var lastHash = parsedHashlist[parsedHashlist.length-1]
              if (lastHash === '') {
                this.hashList += evt.target.result
              } else {
                this.hashList += '\n' + evt.target.result
              }
              this.validateHashes(null)
            }
          }
        }.bind(this)
        reader.readAsText(files[0], 'utf-8')
      },
      submit (forceInvalid) {
        // TODO: maybe delete this condition
        if (this.inputMethod === 'encryptedFile' && !this.$refs.encryptedFileUploader.fileUploaded ) {
          this.$error('No file uploaded.')
          this.step = 1
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

      /*  if (this.attack_settings === "DictAttack" || "pcfg") {
          this.$error('Error in attack settings.')
          return
        }*/

        if (this.hashType === null) {
          this.$error('No hash type selected.')
          this.step = 1
          return
        }

        if (this.name === '') {
          this.$error('Job name can not be empty.')
          return
        }

        this.loading = true
        this.axios.post(this.$serverAddr + '/job', this.jobSettings).then((response) => {
          this.$router.push({name: 'jobDetail', params: {id: response.data.job_id}})
          this.applyTemplate() // Clear all
        }).catch((error) => {
          this.loading = false
        })
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
    bottom: 1em;
    right: 1em;
  }

  .hashCeckContainer {
    display: block;
    max-width: 35px;
    overflow: hidden;
  }

  .textarea.error {
    border-width: 2px;
    border-style: solid;
    border-radius: 5px
  }

  .mode-btn {
    height: initial !important;
    margin: 1em;
  }
</style>
