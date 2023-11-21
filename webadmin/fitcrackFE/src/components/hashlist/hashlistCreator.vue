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
            >
              Hash Extractor
              <v-icon>mdi-eye-lock-open</v-icon>
            </v-tab>
          </v-tabs>

          <v-container>
            <v-row>
              <v-col>
                <v-fade-transition mode="out-in">
                  <v-autocomplete
                    v-if="mode !== 'extract'"
                    id="hash-type-select"
                    v-model="hashType"
                    autofocus
                    clearable
                    label="Select hash type"
                    :items="hashTypes"
                    item-text="name"
                    return-object
                    required
                    hide-details
                    outlined
                    :disabled="mode === 'extract'"
                    no-data-text="No matching hash type"
                  >
                    <template #item="{ item }">
                      <v-list-item-content>
                        <v-list-item-title><b>{{ item.code }}</b> - {{ item.name }}</v-list-item-title>
                      </v-list-item-content>
                    </template>
                  </v-autocomplete>
                  <v-alert
                    v-else
                    type="info"
                    dense
                    text
                    class="my-2"
                  >
                    Hash type will be determined automatically
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
                    >
                    </fc-textarea>
                  </v-col>
                </v-row>
              </v-container>
            </v-tab-item>
            <v-tab-item eager>
              <v-container>
                <v-row>
                  <v-col>
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
    autofocus: Boolean
  },
  data () {
    return {
      modetab: 0,
      hashType: {name: ''},
      hashListManual: '',
      //
      hashTypes: [],
    }
  },
  computed: {
    mode () {
      return ['manual', 'upload', 'extract'][this.modetab]
    },
    valid () {
      switch (this.mode) {
        case "manual":
          return this.hashType.name.length > 0 && this.hashListManual.length > 0
          // should add back validation
        case "upload":
          return this.hashType.name.length > 0 && this.$refs.listUploader.selectedFiles.length == 1
        case "extract":
          return this.$refs.extractUploader.selectedFiles.length == 1
        default:
          return false
      }
    },
    hashesManual () {
      // replace with validation
      return this.hashListManual.split("\n").map(hash => ({hash}))
    },
    hashCode () {
      return parseInt(this.hashType.code)
    }
  },
  watch: {
    valid (newValue) {
      this.$emit('validation', !!newValue)
    }
  },
  methods: {
    getHashTypes () {
      this.axios.get(this.$serverAddr + '/hashcat/hashTypes').then((response) => {
        this.hashTypes = response.data.hashtypes
      })
    },
    createHashlist () {
      return new Promise((resolve, reject) => {
        // reject if invalid
        if (!this.valid) reject("Invalid input")
        // create the empty hashlist
        const url = `${this.$serverAddr}/hashlist`
        console.log(url)
        this.axios.post(url, {name: this.hashlistName}, {withCredentials: true}).then(response => {
          const id = response.data.id
          // add content
          switch (this.mode) {
            case "manual":
              this.axios.post(`${url}/${id}/hashes`, {
                hash_type: this.hashCode.toString(), // WHY
                hash_list: this.hashesManual,
                valid_only: false // TODO
              }, {withCredentials: true}
              ).then(resolve).catch(error => {
                reject(error)
              })
              break
            case "upload":
              this.axios.post(`${url}/${id}/file`, this.$refs.listUploader.files, {
                ...uploadConfig,
                params: {
                  hash_type: this.hashCode,
                  valid_only: true, // TODO
                  binary: false, // TODO
                }
              }
              ).then(resolve).catch(error => {
                reject(error)
              })
              break
            case "extract":
              this.axios.post(`${url}/${id}/extract`, this.$refs.extractUploader.files, {
                ...uploadConfig
                // no params?
              }
              ).then(resolve).catch(error => {
                reject(error)
              })
              break
            default:
              reject("Unknown hash input mode, what the what??")
          }
        }).catch(error => {
          reject(error)
        })
      })
    }
  },
  mounted () {
    this.getHashTypes()
  },
  components: {
    FileUploader,
    fcTextarea
  }
}
</script>