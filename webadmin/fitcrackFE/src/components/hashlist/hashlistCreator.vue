<template>
  <v-container>
    <v-row>
      <v-col>
        <v-card>
          <v-tabs
            v-model="modetab"
            grow
            icons-and-text
          >
            <v-tab
              id="input-manual"
            >
              Manual Input
              <v-icon>mdi-playlist-edit</v-icon>
            </v-tab>
            <v-tab
              id="input-file"
            >
              Hashlist file
              <v-icon>mdi-file-upload</v-icon>
            </v-tab>
            <v-tab
              id="input-extract"
              :disabled="!!existing && !existingTypeCanBeExtracted"
            >
              {{ !!existing && !existingTypeCanBeExtracted ? 'Cannot extract this type' : 'Hash Extractor' }}
              <v-icon>mdi-eye-lock-open</v-icon>
            </v-tab>
          </v-tabs>

          <v-container>
            <v-row>
              <v-col>
                <v-fade-transition mode="out-in">
                  <div v-if="mode !== 'extract'" class="d-flex align-center">
                    <hashTypeAutocomplete
                      v-model="hashType"
                      required
                      outlined
                      dense
                      label="Select hash type"
                      :disabled="mode === 'extract' || !!existing"
                      @input="revalidate"
                    />
                    <!---->
                    <div
                      class="text-overline ml-6 mr-2 text--secondary"
                    >
                      Validation
                    </div>
                    <v-btn-toggle
                      v-model="validationMode"
                      mandatory
                      hide-details
                      dense
                    >
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <v-btn
                            v-on="on"
                            :value="'fail_invalid'"
                          >
                            Strict
                          </v-btn>
                        </template>
                        <span>Any invalid hash encountered cancels the operation completely.</span>
                      </v-tooltip>
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <v-btn
                            v-on="on"
                            :value="'skip_invalid'"
                          >
                            Skipping
                          </v-btn>
                        </template>
                        <span>Only valid hashes are saved, the rest is ignored.</span>
                      </v-tooltip>
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <v-btn
                            v-on="on"
                            :value="'no_validate'"
                          >
                            None
                          </v-btn>
                        </template>
                        <span>No checks are performed before saving the hashes.</span>
                      </v-tooltip>
                    </v-btn-toggle>
                  </div>
                  <!-- when extracting -->
                  <v-alert
                    v-else
                    type="info"
                    dense
                    text
                    class="my-0"
                  >
                    <span v-if="!!existing && existingTypeCanBeExtracted">
                      <strong>Note:</strong> only {{ hashType.name }} will be accepted.
                    </span>
                    <span v-else-if="!!existing && !existingTypeCanBeExtracted">
                      <strong>Note:</strong> extracting hashes of type {{ hashType.name }} is not supported.
                    </span>
                    <span v-else>Hash type is determined automatically when extracting from protected files.</span>
                  </v-alert>
                </v-fade-transition>
              </v-col>
            </v-row>
          </v-container>

          <v-divider></v-divider>

          <v-tabs-items
            v-model="modetab"
          >
            <v-tab-item eager>
              <v-container>
                <v-row>
                  <v-col>
                    <fc-textarea
                      id="hashes-input"
                      ref="textarea"
                      v-model="hashListManual"
                      class="textarea"
                      max-height="500"
                      :can-remove-line="true"
                      :loading="processing"
                      @input="revalidate"
                    >
                      <div
                        slot="after"
                        class="hashCheckContainer pl-1 pt-2"
                      >
                        <div
                          v-for="hashObj in checkedHashesResults"
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
              </v-container>
            </v-tab-item>
            <v-tab-item eager>
              <v-container>
                <v-row>
                  <v-col>
                    <div class="d-flex mx-3 mb-4">
                      <v-checkbox
                        v-model="isBinaryHashlist"
                        label="Treat as a binary hashlist file"
                        hide-details
                      />
                    </div>
                    <file-uploader
                      ref="listUploader"
                      no-upload
                      :multiple="false"
                      label="Select hashlist file to add"
                    />
                  </v-col>
                </v-row>
              </v-container>
            </v-tab-item>
            <v-tab-item eager>
              <v-container>
                <v-row>
                  <v-col>
                    <file-uploader
                    ref="extractUploader"
                    no-upload
                    :multiple="false"
                    label="Select protected file to extract"
                    />
                    <v-alert
                      type="warning"
                    >
                      <strong>Supported formats: </strong>
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <span v-on="on">MS_OFFICE,</span>
                        </template>
                        <span>Hash types: 9400, 9500, 9600, 9700, 9800</span>
                      </v-tooltip>&nbsp;
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <span v-on="on">PDF,</span>
                        </template>
                        <span>Hash types: 10400, 10500, 10600, 10700</span>
                      </v-tooltip>&nbsp;
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <span v-on="on">7Z,</span>
                        </template>
                        <span>Hash type: 11600</span>
                      </v-tooltip>&nbsp;
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <span v-on="on">RAR</span>
                        </template>
                        <span>Hash types: 12500, 13000, 23700, 23800</span>
                      </v-tooltip> and
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <span v-on="on">ZIP.</span>
                        </template>
                        <span>Hash types: 13600, 17200, 17210, 17225, 13001, 23002, 23003</span>
                      </v-tooltip>
                      Hover each to see codes.
                    </v-alert>
                  </v-col>
                </v-row>
              </v-container>
            </v-tab-item>
          </v-tabs-items>
        </v-card>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import FileUploader from "@/components/fileUploader/fileUploader.vue"
import fcTextarea from '@/components/textarea/fc_textarea.vue'
import hashTypeAutocomplete from "@/components/hashlist/hashTypeAutocomplete.vue"

const uploadConfig = {
  withCredentials: true,
  headers: {
    'Content-type': 'multipart/form-data'
  }
}

export default {
  name: "hashlistCreator",
  props: {
    hashlistName: String,
    hashlistId: Number,
    autofocus: Boolean
  },
  data () {
    return {
      processing: false,
      modetab: 0,
      hashType: {name: ''},
      hashListManual: '',
      checkedHashesResults: [],
      checkingError: false,
      existing: null, // used when editing
      isBinaryHashlist: false,
      validationMode: 'fail_invalid',
      //
      extractableHashTypes: [
                              9400, 9500, 9600, 9700, 9800,
                              10400, 10500, 10600, 10700,
                              11600,
                              12500, 13000, 23700, 23800,
                              13600, 17200, 17210, 17225, 13001, 23002, 23003
                            ]
    }
  },
  computed: {
    mode () {
      return ['manual', 'upload', 'extract'][this.modetab]
    },
    validNameOrId () {
      return this.hashlistName.length > 0 || typeof this.hashlistId === 'number'
    },
    validManualEntry () {
      if (this.checkingError && this.validationMode === 'fail_invalid') return false
      const validCount = this.checkedHashesResults.filter(item => item.result === "OK").length
      if (this.validationMode === 'skip_invalid') {
        return validCount > 0
      } else {
        return this.hashListManual.length > 0
      }
    },
    valid () {
      switch (this.mode) {
        case "manual":
          return this.validNameOrId && this.hashCode != null && this.validManualEntry
        case "upload":
          return this.validNameOrId && this.hashCode != null && this.$refs.listUploader.selectedFiles.length == 1
        case "extract":
          return this.validNameOrId && this.$refs.extractUploader.selectedFiles.length == 1
        default:
          return false
      }
    },
    hashesManual () {
      return this.hashListManual.split("\n").map(hash => ({hash}))
    },
    hashCode () {
      return this.hashType ? parseInt(this.hashType.code) : null
    },
    existingTypeCanBeExtracted () {
      if (!this.existing) return false
      return this.extractableHashTypes.includes(this.hashCode)
    },
    id () {
      return this.hashlistId || 'new'
    }
  },
  watch: {
    valid (newValue) {
      this.$emit('validation', !!newValue)
    }
  },
  mounted () {
    if (this.id !== 'new') {
      this.loadExistingHahslistInfo()
    }
  },
  methods: {
    async loadExistingHahslistInfo () {
      const hashtypes = await this.axios.get(this.$serverAddr + '/hashcat/hashTypes').then((response) => response.data.hashtypes)
      this.axios.get(`${this.$serverAddr}/hashlist/${this.id}`).then(response => {
        const data = response.data
        this.hashType = hashtypes.find(ht => ht.name === data.hash_type_name)
        this.existing = data
      })
    },
    revalidate () {
      this.processing = true
      //
      this.checkedHashesErrors = []
      this.checkedHashesResults = []
      //
      this.axios.post(this.$serverAddr + '/job/verifyHash', {
          'hashtype': this.hashCode,
          'hashes': this.hashListManual
        }).then((response) => {
          this.checkingError = response.data.error
          this.checkedHashesResults = response.data.items
        }).finally(() => {
          this.processing = false
        })
    },
    createHashlist () {
      return new Promise((resolve, reject) => {
        // reject if invalid
        if (!this.valid) reject("Invalid input")
        //
        const url = `${this.$serverAddr}/hashlist`
        switch (this.mode) {
            case "manual":
              this.axios.post(`${url}/${this.id}/hashes`, {
                name: this.hashlistName,
                hash_type: this.hashCode,
                hash_list: this.hashesManual,
                validation_mode: this.validationMode
              }, {withCredentials: true}
              ).then(resolve).catch(error => {
                reject(error)
              })
              break
            case "upload":
              this.axios.post(`${url}/${this.id}/file`, this.$refs.listUploader.files, {
                ...uploadConfig,
                params: {
                  name: this.hashlistName,
                  hash_type: this.hashCode,
                  validation_mode: this.validationMode,
                  binary: this.isBinaryHashlist,
                }
              }
              ).then(resolve).catch(error => {
                reject(error)
              })
              break
            case "extract":
              this.axios.post(`${url}/${this.id}/extract`, this.$refs.extractUploader.files, {
                ...uploadConfig,
                params: {
                  name: this.hashlistName
                }
              }
              ).then(resolve).catch(error => {
                reject(error)
              })
              break
            default:
              reject("Unknown hash input mode, what the what??")
          }
      })
    }
  },
  components: {
    FileUploader,
    fcTextarea,
    hashTypeAutocomplete
  }
}
</script>

<style>
.hashCheckContainer {
  display: block;
  width: 2em;
  overflow: hidden;
}
</style>