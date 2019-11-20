<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="containerAddJob height100 mx-auto">
    <v-app-bar
      app
      height="48px"
      class="topToolbar"
      :class="{scrollTop: (!knowEstimatedTime && !showEstimatedTime)}"
    >
      <v-toolbar-title class="estimatedTime">
        Keyspace: {{ keyspace }}. Estimated cracking time is {{ estimatedTime }}.
      </v-toolbar-title>
    </v-app-bar>
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

    <h1 class="mb-4">
      Create a new job
    </h1>

    <v-text-field
      v-model="name"
      label="Name"
      outlined
      autofocus
      required
    />

    <v-textarea
      v-model="comment"
      label="Comment (optional)"
      outlined
    />

    <div>
      <v-col class="addJobContent mx-auto">
        <v-stepper
          v-model="step"
          vertical
          class="mb-4"
        >
          <v-stepper-step 
            editable
            step="1"
          >
            Input settings
          </v-stepper-step>
          <v-stepper-content step="1">
            <v-row class="px-0 pt-2">
              <v-col cols="4">
                <v-subheader class="height100">
                  Hashtype:
                </v-subheader>
              </v-col>
              <v-col cols="8">
                <v-autocomplete
                  v-model="hashtype"
                  class="hashtypeSelect pr-2"
                  editable
                  validate-on-blur
                  clearable
                  multi-line
                  label="Select hash type"
                  :items="hashTypes"
                  item-text="name"
                  return-object
                  required
                  dense
                  outlined
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
              </v-col>
              <v-col
                v-if="hashtype && hashtype.subcategories !== null"
                cols="12"
                class="pt-3"
              >
                <v-col
                  v-for="(value, key) in hashtype.subcategories"
                  class="d-flex"
                  cols="12"
                >
                  <v-select
                    return-object
                    required
                    hide-details
                    class="px-3 pt-2"
                    height="24"
                    offset-y
                    @change="subHashtypeChanged(key, $event)"
                    item-text="description"
                    :items="value"
                    :label="key"
                    outlined
                    single-line
                  >
                    <template
                      slot="item"
                      slot-scope="data"
                    >
                      <v-list-item-content>
                        <v-list-item-title><b>{{ data.item.code }}</b> - {{ data.item.description }}</v-list-item-title>
                      </v-list-item-content>
                    </template>
                  </v-select>
                </v-col>
              </v-col>

              <v-col cols="4">
                <v-subheader class="height100">
                  Upload method:
                </v-subheader>
              </v-col>
              <v-col cols="8">
                <v-radio-group v-model="inputMethod">
                  <v-radio
                    value="extractFromFile"
                    label="Extract from file"
                  />
                  <v-radio
                    value="hashFile"
                    label="Upload hash files"
                  />
                  <v-radio
                    value="multipleHashes"
                    label="Enter hashes"
                    @click.native="focusTextarea"
                  />
                </v-radio-group>
              </v-col>
              <v-col
                v-if="inputMethod === 'extractFromFile'"
                cols="12"
              >
                <v-alert
                  type="warning"
                  class="mt-0 mb-0"
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
                    <v-row
                      v-for="hashObj in validatedHashes"
                      justify="end"
                      class="pa-0 ma-0"
                    >
                      <v-col cols="6">
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
                      </v-col>

                      <v-col cols="6">
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
                      </v-col>
                    </v-row>
                  </div>
                </fc-textarea>
              </v-col>
            </v-row>
          </v-stepper-content>

          <v-stepper-step 
            editable
            step="2"
          >
            Attack settings
          </v-stepper-step>
          <!--    <h3> Message: {{ attackSettings }} </h3> -->
          <v-stepper-content step="2">
            <v-tabs
              v-model="attackSettingsTab"
              color="primary"
              class="mx-0"
            >
              <v-tab
                @click="attackTabChanged($refs.DictAttack)"
              >
                Dictionary
              </v-tab>
              <v-tab-item>
                <v-card text>
                  <dictionary
                    ref="DictAttack"
                    v-model="attackSettings"
                  />
                </v-card>
              </v-tab-item>
              <v-tab
                @click="attackTabChanged($refs.CombAttack)"
              >
                Combination
              </v-tab>
              <v-tab-item>
                <v-card text>
                  <combinator
                    ref="CombAttack"
                    v-model="attackSettings"
                  />
                </v-card>
              </v-tab-item>
              <v-tab
                @click="attackTabChanged($refs.BruteAttack)"
              >
                Brute-force
              </v-tab>
              <v-tab-item>
                <v-card text>
                  <maskattack
                    ref="BruteAttack"
                    v-model="attackSettings"
                  />
                </v-card>
              </v-tab-item>
              <v-tab
                @click="attackTabChanged($refs.HybridWordMaskAttack)"
              >
                Hybrid Wordlist + Mask
              </v-tab>
              <v-tab-item>
                <v-card text>
                  <hybridWordlistMask
                    ref="HybridWordMaskAttack"
                    v-model="attackSettings"
                  />
                </v-card>
              </v-tab-item>
              <v-tab
                @click="attackTabChanged($refs.HybridMaskWordAttack)"
              >
                Hybrid Mask + Wordlist
              </v-tab>
              <v-tab-item>
                <v-card text>
                  <hybridMaskWordlist
                    ref="HybridMaskWordAttack"
                    v-model="attackSettings"
                  />
                </v-card>
              </v-tab-item>
              <v-tab
                @click="attackTabChanged($refs.pcfg)"
              >
                PCFG
              </v-tab>
              <v-tab-item>
                <v-card text>
                  <pcfg
                    ref="pcfgAttack"
                    v-model="attackSettings"
                  />
                </v-card>
              </v-tab-item>
            </v-tabs>
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
          </v-stepper-content>
          <v-stepper-step 
            editable
            step="4"
          >
            Additional settings
          </v-stepper-step>
          <v-stepper-content step="4">
            <v-row class="px-3">
              <v-col cols="6">
                <v-subheader class="height100">
                  Desired time for each job:
                </v-subheader>
              </v-col>
              <v-col cols="6">
                <v-text-field
                  v-model="timeForJob"
                  text
                  single-line
                  label="Time for job"
                  required
                  type="number"
                  suffix="seconds"
                  :min="60"
                />
              </v-col>
              <v-col cols="4">
                <v-subheader class="height100">
                  Start:
                </v-subheader>
              </v-col>
              <v-col cols="5">
                <v-text-field
                  v-model="startDate"
                  :disabled="startNow"
                  text
                  single-line
                  label=""
                  mask="date-with-time"
                />
              </v-col>
              <v-col cols="3">
                <v-checkbox
                  v-model="startNow"
                  label="start now"
                />
              </v-col>
              <v-col cols="4">
                <v-subheader class="height100">
                  End:
                </v-subheader>
              </v-col>
              <v-col cols="5">
                <v-text-field
                  v-model="endDate"
                  :disabled="endNever"
                  text
                  single-line
                  label=""
                  mask="date-with-time"
                />
              </v-col>
              <v-col cols="3">
                <v-checkbox
                  v-model="endNever"
                  label="End never"
                />
              </v-col>
            </v-row>
          </v-stepper-content>
        </v-stepper>

        <v-row
          justify="center"
          class="mb-5"
        >
          <template-modal :data="jobSettings" />
          <v-btn
            large
            color="primary"
            class="ml-2"
            @click="submit"
          >
            <v-icon left>
              mdi-check
            </v-icon>Submit
          </v-btn>
          <!--<template-modal :data="sendingJson"></template-modal>-->
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
  import pcfg from '@/components/job/attacks/pcfg'
  import FileUploader from "@/components/fileUploader/fileUploader";
  import fcTextarea from '@/components/textarea/fc_textarea'
  import hostSelector from '@/components/selector/hostSelector'
  import templateModal from '@/components/jobTemplate/templateModal'
  export default {
    name: 'AddJob',
    components: {
      FileUploader,
      'combinator': combinator,
      'maskattack': mask,
      'dictionary': dictionary,
      'hybridMaskWordlist': hybridMaskWordlist,
      'hybridWordlistMask': hybridWordlistMask,
      'pcfg': pcfg,
      'fc-textarea': fcTextarea,
      'host-selector': hostSelector,
      'template-modal': templateModal
    },
    data: function () {
      return {
        step: 1,
        attackSettingsTab: null,
        attackSettings: false,
        loading: false,
        valid: true,
        name: '',
        comment: '',
        hashtype: null,
        hashTypes: [],
        timeForJob: 3600,
        hashList: '',
        inputMethod: null,
        hosts: [],
        showEstimatedTime: false,
        estimatedTime: '',
        keyspace: null,
        startDate: this.$moment().format('DD/MM/YYYY HH:mm'),
        startNow: true,
        endDate: this.$moment().format('DD/MM/YYYY HH:mm'),
        endNever: true,
        validatedHashes: [],
        gotBinaryHash: false,
        hashListError: false
      }
    },
    computed: {
      jobSettings () {
        return {
          "name": this.name,
          "comment": this.comment,
          "priority": 0,
          "hosts_ids": this.hosts.map(h => h.id),
          "seconds_per_job": parseInt(this.timeForJob),
          "time_start": (this.startNow ? '' : this.startDate),
          "time_end": (this.endNever ? '' : this.endDate),
          'attack_settings': this.attackSettings,
          "hash_settings": {
            "hash_type": this.hashtype ? this.hashtype.code : null,
            "hash_list": this.validatedHashes
          }
        }
      },
      sendingJson: function () {
        return {
          'hosts_ids': this.hosts,
          'seconds_per_job': parseInt(this.timeForJob),
          'attack_settings': this.attackSettings,
          'hash_type': this.hashtype

        }
      },
      knowEstimatedTime: function () {
        return !!this.estimatedTime
      },
    },
    watch: {
      jobSettings () {
        if ( typeof this.attackSettings !== 'object' || this.hashtype == null || this.hosts.length === 0) {
          this.showEstimatedTime = false
        } else {
          var boincIds = []
          for (let i = 0; i < this.hosts.length; i++) {
            boincIds.push(this.hosts[i].id)
          }

          this.axios.get(this.$serverAddr + '/job/crackingTime', {
            params: {
              'hash_type_code': this.hashtype,
              'boinc_host_ids': boincIds.join(","),
              'attack_settings': this.attackSettings
            }
          }).then((response) => {
            console.log(response)
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
      // this.axios.get(this.$serverAddr + '/template/1', ).then((response) => {
      //   setTimeout(function(){
      //     var data = JSON.parse(response.data.data)
      //     console.log( data.attack_settings)
      //     this.$set(this, 'attackSettings', data.attack_settings)
      //
      //   }.bind(this), 1000);
      //
      // })
    },
    methods: {
      subHashtypeChanged: function (key, val) {
        this.hashtype.code = this.hashtype.code.replace(key, val.code)
        this.validateHashes(null)
      },
      attackTabChanged: function (tab) {
      //  if (tab === undefined) {
          this.attackSettings = false
      //  } else {
          tab.checkValid()
      //  }
      },
      focusTextarea: function () {
        this.$refs.textarea.focus()
      },
      validateHashes: function (data) {
        if (data === null) {
          data = this.hashList
        }
        var hashesList = data.split('\n')
        if (data.startsWith("BASE64:")) {
          this.gotBinaryHash = true
        } else {
          this.gotBinaryHash = false
        }
        if (this.hashtype === null || isNaN(this.hashtype.code)) {
          return
        }
        if (data === '') {
          return
        }

        this.axios.post(this.$serverAddr + '/job/verifyHash', {
          'hashtype': this.hashtype.code,
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
          this.hashList += hash
        } else {
          this.hashList += '\n' + hash
        }
      },
      uploadComplete: function (data) {
        this.$success("Successfully extracted hash form file.")
        this.hashtype = this.hashTypes.find(h => h.code == data['hash_type'])
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
      submit: function () {
        for (var i = 0; i < this.validatedHashes.length; i++) {
          if (this.validatedHashes[i].result !== 'OK') {
            this.$error('Wrong hash format - ' + this.validatedHashes[i].hash)
            return
          }
        }

        // TODO: maybe delete this condition
        if (this.inputMethod === 'encryptedFile' && !this.$refs.encryptedFileUploader.fileUploaded ) {
          this.$error('No file uploaded.')
          return
        }

        if (this.attack_settings === false) {
          this.$error('Error in attack settings.')
          return
        }

      /*  if (this.attack_settings === "DictAttack" || "pcfg") {
          this.$error('Error in attack settings.')
          return
        }*/

        if (this.hashtype === null) {
          this.$error('No hash type selected.')
          return
        }

        if (this.name === '') {
          this.$error('Job name can not be empty.')
          return
        }

        if (this.inputMethod === false) {
          this.$error('No input method selected.')
          return
        }

        this.loading = true
        this.axios.post(this.$serverAddr + '/job', this.jobSettings).then((response) => {
          this.$router.push({name: 'jobDetail', params: {id: response.data.job_id}})
          console.log(response.data)
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
    max-width: 1185px;
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


  .topToolbar {
    z-index: 2;
    top: 64px;
    transform: translateY(0px) !important;
  }

  .uploadBtn {
    margin: auto;
    display: block;
  }

  .scrollTop {
    opacity: 0;
    transform: translateY(-48px) !important;
  }

  .estimatedTime {
    font-weight: normal;
    font-size: 16px;
  }


  .height100 {
    height: 100%;
  }

  .height64 {
    height: 64px;
  }

  .hashtypeSelect {
    height: 50px;
  }

  .noEvent {
    pointer-events: none;
  }

  .dz-message {
    cursor: pointer;
  }

  .width100 {
    width: 100%;
  }

  .maskBtn {
    min-width: 0px;
    max-width: 40px;
    text-transform: none;
  }

  .maskTitle {
    font-size: 18px;
  }

  .loadingProgressCont {
    position: fixed;
    width: 100%;
    height: 100%;
    top: 0;
    right: 0;
    left: 0;
    bottom: 0;
    margin: auto;
    z-index: 2;

  }

  .loadingProgress {
    position: absolute;
    top: 0;
    right: 0;
    left: 0;
    bottom: 0;
    margin: auto;
  }




  .oneline {
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
    display: block;
    width: 200px;
    line-height: 50px;
    height: 50px;
  }

  .inheritColor {
    color: inherit !important;
  }

  .hashCeckContainer {
    display: block;
    max-width: 35px;
    overflow: hidden;
  }

  .textarea.error {
    border-width: 2px;
    border-style: solid;
  }

  .width15 {
    width: 15px;
  }
</style>

<style>
  .selectedDict {
    background: rgba(37, 157, 173, 0.85) !important;
    color: white;
  }

  .selectedDict a {
    color: white;
  }

  .clickable {
    cursor: pointer;
  }

  .attackSettings .tabs__bar {
    box-shadow: 0 2px 4px -1px rgba(0,0,0,.2), 0 4px 5px 0 rgba(0,0,0,.14), 0 1px 10px 0 rgba(0,0,0,.12);
    z-index: 1;
  }
</style>
