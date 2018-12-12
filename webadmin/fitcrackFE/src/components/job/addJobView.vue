<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="containerAddJob height100 mx-auto">
    <v-toolbar app dark color="primary" height="48px" class="topToolbar"
               v-bind:class="{scrollTop: (!knowEstimatedTime && !showEstimatedTime)}">
      <v-toolbar-title class="estimatedTime">Estimated cracking time is {{estimatedTime}}.
      </v-toolbar-title>
    </v-toolbar>
    <div class="loadingProgressCont" v-show="loading">
      <v-progress-circular
        indeterminate
        color="primary"
        size="65"
        class="loadingProgress"
        :width="4"/>
    </div>
    <div>
      <v-flex class="addJobContent mx-auto">
        <v-form v-model="valid" ref="form">
          <v-layout row wrap>
            <v-flex xs6 class="px-2">
              <div class="max500 mx-auto mb-5">
              <fc-tile title="Create new job">
                <v-layout row wrap class="px-3">
                  <v-flex xs4>
                    <v-subheader class="height64">Name:</v-subheader>
                  </v-flex>
                  <v-flex xs8>
                    <v-text-field
                      single-line
                      label="Name"
                      v-model="name"
                      required
                    ></v-text-field>
                  </v-flex>
                  <v-flex xs4>
                    <v-subheader class="height64">Comment:</v-subheader>
                  </v-flex>
                  <v-flex xs8>
                    <v-text-field
                      label="Comment"
                      v-model="comment"
                    ></v-text-field>
                  </v-flex>
                </v-layout>
              </fc-tile>
            </div>
            </v-flex>
            <v-flex xs6 class="px-2">
              <div class="max500 mx-auto mb-5">
              <fc-tile title="Input settings">
                <v-layout row wrap class="px-0 pt-2">
                  <v-flex xs4>
                    <v-subheader class="height100">Hashtype:</v-subheader>
                  </v-flex>
                  <v-flex xs8>
                    <v-autocomplete
                      class="hashtypeSelect pr-2"
                      editable
                      validate-on-blur
                      clearable
                      multi-line
                      label="Hashtype"
                      v-model="hashtype"
                      :items="hashTypes"
                      item-text="name"
                      return-object
                      @change="validateHashes(null)"
                      required
                      dense
                      box
                    >
                      <template
                        slot="item"
                        slot-scope="data"
                      >
                        <v-list-tile-content>
                          <v-list-tile-title><b>{{data.item.code}}</b> - {{data.item.name}}</v-list-tile-title>
                        </v-list-tile-content>
                      </template>
                    </v-autocomplete>
                  </v-flex>
                  <v-flex xs12 v-if="hashtype && hashtype.subcategories !== null" class="pt-3">
                    <v-flex xs12 d-flex v-for="(value, key) in hashtype.subcategories">
                      <v-select
                        @change="subHashtypeChanged(key, $event)"
                        return-object
                        required
                        hide-details
                        class="px-3 pt-2"
                        height="24"
                        offset-y
                        item-text="description"
                        :items="value"
                        :label="key"
                        outline
                        single-line
                      >
                        <template
                          slot="item"
                          slot-scope="data"
                        >
                          <v-list-tile-content>
                            <v-list-tile-title><b>{{data.item.code}}</b> - {{data.item.description}}</v-list-tile-title>
                          </v-list-tile-content>
                        </template>
                      </v-select>
                    </v-flex>
                  </v-flex>

                  <v-flex xs4>
                    <v-subheader class="height100">Upload method:</v-subheader>
                  </v-flex>
                  <v-flex xs8>
                    <v-radio-group v-model="inputMethod">
                      <v-radio value="extractFromFile" label="Extract from file"></v-radio>
                      <v-radio value="hashFile" label="Upload hash files"></v-radio>
                      <v-radio value="multipleHashes" label="Enter hashes" @click.native="focusTextarea"></v-radio>
                    </v-radio-group>
                  </v-flex>
                  <v-flex xs12 v-if="inputMethod === 'extractFromFile'">
                    <v-divider></v-divider>
                    <v-alert :value="true" type="warning" class="mt-0 mb-0" >
                      Currently we supports these formats:
                      <v-tooltip top>
                        <span slot="activator"><a href="#">MS_OFFICE</a>,</span>
                        <span>Hashtypes: 9400, 9500, 9600, 9700, 9800</span>
                      </v-tooltip>
                      <v-tooltip top>
                        <span slot="activator"><a href="#">PDF</a>,</span>
                        <span>Hashtypes: 10400, 10500, 10600, 10700</span>
                      </v-tooltip>
                      <v-tooltip top>
                        <span slot="activator"><a href="#">RAR</a> and </span>
                        <span>Hashtypes: 12500, 13000</span>
                      </v-tooltip>
                      <v-tooltip top>
                        <span slot="activator"><a href="#">ZIP</a>.</span>
                        <span>Hashtypes: 13600</span>
                      </v-tooltip>
                    </v-alert>
                    <file-uploader ref="encryptedFileUploader" :url="this.$serverAddr + '/protectedFiles/add'" @uploadComplete="uploadComplete"></file-uploader>
                  </v-flex>
                  <v-flex xs12 v-if="inputMethod === 'hashFile'">
                    <v-alert :value="true" type="warning" class="mt-0 mb-0" >
                      Input to hashcat. Can be binary hash (WPA/WPA2), or hashlist.
                    </v-alert>
                    <file-uploader ref="hashFileUploader" :noUpload="true" @filesChanged="hashFileSelected"></file-uploader>
                  </v-flex>
                  <v-flex xs12>
                    <v-alert :value="true" type="info" color="primary" class="mt-0 mb-0" v-if="gotBinaryHash" >You can select only one binary hash.</v-alert>
                    <fc-textarea
                      :class="{error: hashListError}"
                      class="textarea"
                      v-if="inputMethod !== null"
                      ref="textarea"
                      maxHeight="500"
                      :readonly="!(inputMethod === 'multipleHashes' && !gotBinaryHash) "
                      v-model="hashList"
                      @blur="validateHashes"
                      @focus="unvalidateHashes"
                      :canRemoveLine="true "
                    >
                      <div class="hashCeckContainer pl-1 pt-2"  slot="after">
                        <p class="pa-0 ma-0" v-for="hashObj in validatedHashes" >
                          <v-icon small v-if="hashObj.result === 'OK'" color="success">check_circle_outline</v-icon>
                          <v-tooltip left v-else>
                            <v-icon small slot="activator" color="error" class="clickable">error_circle_outline</v-icon>
                            <span>{{hashObj.result}}</span>
                          </v-tooltip>
                        </p>
                      </div>
                    </fc-textarea>
                  </v-flex>
                </v-layout>
              </fc-tile>
            </div>
            </v-flex>
          </v-layout>

          <div class="max1000 mx-auto mb-5">
            <fc-tile title="Attack settings">
              <v-layout row wrap>
                <v-tabs
                  class="width100 attackSettings"
                  xs12
                  v-model="attackSettingsTab"
                  centered
                  show-arrows
                  color="primary darken-1"
                  dark
                  slider-color="white"
                >
                  <v-tab  :ripple="false" @click="attackTabChanged($refs.DictAttack)">
                    Dictionary attack
                  </v-tab>
                  <v-tab-item lazy >
                    <v-card flat>
                      <dictionary v-model="attackSettings" ref="DictAttack"></dictionary>
                    </v-card>
                  </v-tab-item>
                  <v-tab  :ripple="false" @click="attackTabChanged($refs.CombAttack)">
                    Combination attack
                  </v-tab>
                  <v-tab-item lazy>
                    <v-card flat>
                      <combinator v-model="attackSettings" ref="CombAttack"></combinator>
                    </v-card>
                  </v-tab-item>
                  <v-tab  :ripple="false" @click="attackTabChanged($refs.BruteAttack)">
                    Brute-force attack
                  </v-tab>
                  <v-tab-item lazy>
                    <v-card flat>
                      <maskattack v-model="attackSettings" ref="BruteAttack"></maskattack>
                    </v-card>
                  </v-tab-item>
                  <v-tab  :ripple="false" @click="attackTabChanged($refs.HybridWordMaskAttack)">
                    Hybrid Wordlist + Mask
                  </v-tab>
                  <v-tab-item lazy>
                    <v-card flat>
                      <hybridWordlistMask v-model="attackSettings" ref="HybridWordMaskAttack"></hybridWordlistMask>
                    </v-card>
                  </v-tab-item>
                  <v-tab :ripple="false" @click="attackTabChanged($refs.HybridMaskWordAttack)">
                    Hybrid Mask + Wordlist
                  </v-tab>
                  <v-tab-item lazy>
                    <v-card flat>
                      <hybridMaskWordlist v-model="attackSettings" ref="HybridMaskWordAttack"></hybridMaskWordlist>
                    </v-card>
                  </v-tab-item>
                </v-tabs>
              </v-layout>
            </fc-tile>
          </div>

          <div class="max800 mx-auto mb-5">
            <fc-tile title="Host mapping">
              <v-layout row wrap class="">
                <host-selector :selectAll="true" v-model="hosts"></host-selector>
              </v-layout>
            </fc-tile>
          </div>


          <div class="max500 mx-auto mb-5">
            <fc-tile title="Miscellanious settings">
              <v-layout row wrap class="px-3">
                <v-flex xs6>
                  <v-subheader class="height100">Desired time for each job:</v-subheader>
                </v-flex>
                <v-flex xs6>
                  <v-text-field
                    flat
                    single-line
                    label="Time for job"
                    v-model="timeForJob"
                    required
                    type="number"
                    suffix="seconds"
                    :min="60"
                  ></v-text-field>
                </v-flex>
                <v-flex xs4>
                  <v-subheader class="height100">Start:</v-subheader>
                </v-flex>
                <v-flex xs5>
                  <v-text-field
                    :disabled="startNow"
                    v-model="startDate"
                    flat
                    single-line
                    label=""
                    mask="date-with-time"
                  ></v-text-field>
                </v-flex>
                <v-flex xs3>
                  <v-checkbox
                    label="start now"
                    v-model="startNow"
                  ></v-checkbox>
                </v-flex>
                <v-flex xs4>
                  <v-subheader class="height100">End:</v-subheader>
                </v-flex>
                <v-flex xs5>
                  <v-text-field
                    :disabled="endNever"
                    v-model="endDate"
                    flat
                    single-line
                    label=""
                    mask="date-with-time"
                  ></v-text-field>
                </v-flex>
                <v-flex xs3>
                  <v-checkbox
                    label="End never"
                    v-model="endNever"
                  ></v-checkbox>
                </v-flex>
              </v-layout>
            </fc-tile>
          </div>
          <v-layout row justify-center class="mb-5">
            <v-btn
              large
              color="primary"
              @click="submit"

            >Submit
            </v-btn>
            <!--<template-modal :data="sendingJson"></template-modal>-->
          </v-layout>

        </v-form>

      </v-flex>

    </div>
  </div>
</template>

<script>
  import combinator from '@/components/job/attacks/combinator'
  import mask from '@/components/job/attacks/mask'
  import dictionary from '@/components/job/attacks/dictionary'
  import hybridMaskWordlist from '@/components/job/attacks/hybridMaskWordlist'
  import hybridWordlistMask from '@/components/job/attacks/hybridWordlistMask'
  import tile from '@/components/tile/fc_tile'
  import FileUploader from "@/components/fileUploader/fileUploader";
  import fcTextarea from '@/components/textarea/fc_textarea'
  import hostSelector from '@/components/selector/hostSelector'
  import templateModal from '@/components/jobTemplate/templateModal'
  export default {
    name: 'addJob',
    components: {
      FileUploader,
      'fc-tile': tile,
      'combinator': combinator,
      'maskattack': mask,
      'dictionary': dictionary,
      'hybridMaskWordlist': hybridMaskWordlist,
      'hybridWordlistMask': hybridWordlistMask,
      'fc-textarea': fcTextarea,
      'host-selector': hostSelector,
      'template-modal': templateModal
    },
    data: function () {
      return {
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
        startDate: this.$moment().format('DD/MM/YYYY HH:mm'),
        startNow: true,
        endDate: this.$moment().format('DD/MM/YYYY HH:mm'),
        endNever: true,
        validatedHashes: [],
        gotBinaryHash: false,
        hashListError: false
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
    computed: {
      sendingJson: function () {
        return {
          'hosts_ids': this.hosts,
          'seconds_per_job': parseInt(this.timeForJob),
          'attack_settings': this.attackSettings,
          'hash_type': this.hashtype

        }
      },
      knowEstimatedTime: function () {
        if ( typeof this.attackSettings !== 'object' || this.hashtype == null || this.hosts.length === 0)
          this.showEstimatedTime = false
        else {
          var boincIds = []
          for (let i = 0; i < this.hosts.length; i++) {
            boincIds.push(this.hosts[i].id)
          }

          this.axios.get(this.$serverAddr + '/jobs/crackingTime', {
            params: {
              'hash_type_code': this.hashtype,
              'boinc_host_ids': boincIds.join(","),
              'attack_settings': this.attackSettings
            }
          }).then((response) => {
            console.log(response)
            if (response['data']) {
              this.estimatedTime = response.data.display_time
              this.showEstimatedTime = true
            }
          })
        }
      },
    },
    methods: {
      subHashtypeChanged: function (key, val) {
        this.hashtype.code = this.hashtype.code.replace(key, val.code)
        this.validateHashes(null)
      },
      attackTabChanged: function (tab) {
        if (tab === undefined) {
          this.attackSettings = false
        } else {
          tab.checkValid()
        }
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

        this.axios.post(this.$serverAddr + '/jobs/verifyHash', {
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
        this.hashtype = this.hashTypes.find( hashtype => {
          return hashtype.code === data['hash_type'];
        })
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
        var hostIds = []
        for (let i = 0; i < this.hosts.length; i++) {
          hostIds.push(this.hosts[i].id)
        }
        this.axios.post(this.$serverAddr + '/jobs', {
          "name": this.name,
          "comment": this.comment,
          "priority": 0,
          "hosts_ids": hostIds,
          "seconds_per_job": parseInt(this.timeForJob),
          "time_start": (this.startNow ? '' : this.startDate),
          "time_end": (this.endNever ? '' : this.endDate),
          'attack_settings': this.attackSettings,
          "hash_settings": {
            "hash_type": this.hashtype.code,
            "hash_list": this.validatedHashes
          }
        }).then((response) => {
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


  .addBtn {
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
    background: white;
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
    vertical-align: middle;
    line-height: 50px;
    height: 50px;
  }

  .inheritColor {
    color: inherit !important;
  }

  .hashCeckContainer {
    display: block;
    max-width: 20px;
    overflow: hidden;
  }

  .textarea.error {
    border-width: 2px;
    border-style: solid;
    background: white !important;
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
