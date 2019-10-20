<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max500">
    <fc-tile
      title="Markov chains"
      class="ma-2"
    >
      <v-alert
        :value="true"
        type="warning"
        class="mt-0 mb-1"
      >
        Markov files must have a .hcstat extension.
      </v-alert>
      <v-data-table
        :headers="headers"
        :items="markovChains.items"
        :loading="loading"
        :footer-props="{itemsPerPageOptions: [10,25,50], itemsPerPageText: 'Hcstats per page'}"
      >
        <template
          slot="items"
          slot-scope="props"
        >
          <td>{{ props.item.name }}</td>
          <td class="text-right">
            {{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}
          </td>
          <td class="text-right">
            <a
              :href="$serverAddr + '/markovChains/' + props.item.id"
              target="_blank"
            >
              <v-btn
                outlined
                fab
                small
                color="primary"
              >
                <v-icon>file_download</v-icon>
              </v-btn>
            </a>
          </td>
          <td class="text-right">
            <v-tooltip top>
              <template v-slot:activator="{ on }">
                <v-btn
                  icon
                  class="mx-0"
                  @click="deleteMarkov(props.item.id)"
                  v-on="on"
                >
                  <v-icon color="error">
                    delete
                  </v-icon>
                </v-btn>
              </template>
              <span>Delete Markov file</span>
            </v-tooltip>
          </td>
        </template>
      </v-data-table>
      <div class="text-right pa-3">
        <v-btn
          class="d-inline-block"
          color="primary"
          text
          outlined
          @click.native.stop="dialog = true; loadDictionaries()"
        >
          Add new
        </v-btn>
      </div>
    </fc-tile>








    <v-dialog
      v-model="dialog"
      max-width="600"
      lazy
    >
      <v-card>
        <v-card-title class="headline">
          Add new HcStat
        </v-card-title>
        <v-tabs
          v-model="active"
          color="grey lighten-4"
          light
          slider-color="primary"
        >
          <v-tab ripple>
            Upload file
          </v-tab>
          <v-tab ripple>
            Make from dictionary
          </v-tab>
          <v-tab-item>
            <v-card text>
              <file-uploader
                :url="this.$serverAddr + '/markovChains/add'"
                @uploadComplete="loadMarkovChains(); dialog=false"
              />
            </v-card>
          </v-tab-item>
          <v-tab-item>
            <div
              v-if="adding"
              class="text-center"
            >
              <v-progress-circular
                class="ma-5"
                size="50"
                :width="3"
                indeterminate
                color="primary"
              />
            </div>
            <v-card
              v-else
              text
            >
              <div class="px-2">
                <v-text-field
                  id="newHcStatName"
                  v-model="newName"
                  name="name"
                  label="Name"
                />
              </div>
              <v-data-table
                :headers="dictionaryHeader"
                :items="dictionaries.items"
                item-key="id"
                :loading="loadingDictionaries"
                :footer-props="{itemsPerPageOptions: [10,25,50], itemsPerPageText: 'Dictionaries per page'}"
              >
                <template
                  slot="items"
                  slot-scope="props"
                >
                  <tr
                    class="clickable"
                    :class="{selectedDict: (props.item.id === selectedDictId)}"
                    @click="selectedDictId=props.item.id"
                  >
                    <td>{{ props.item.name }}</td>
                    <td class="text-right">
                      {{ props.item.keyspace }}
                    </td>
                    <td class="text-right">
                      {{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}
                    </td>
                    <td class="text-right">
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <v-btn
                            icon
                            class="mx-0"
                            :to="{name: 'dictionaryDetail', params: { id: props.item.id}}"
                            v-on="on"
                          >
                            <v-icon color="primary">
                              link
                            </v-icon>
                          </v-btn>
                        </template>
                        <span>Go to the dictionary page</span>
                      </v-tooltip>
                    </td>
                  </tr>
                </template>
              </v-data-table>
              <div class="text-right pa-3">
                <v-btn
                  class="d-inline-block"
                  color="primary"

                  outlined
                  :disabled="selectedDictId === null && newName === ''"
                  @click="makeHcStatFromDictionary()"
                >
                  Make from dictionary
                </v-btn>
              </div>
            </v-card>
          </v-tab-item>
        </v-tabs>
      </v-card>
    </v-dialog>
  </v-container>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import FileUploader from "@/components/fileUploader/fileUploader";
  export default {
    name: "MarkovView",
    components: {
      FileUploader,
      'fc-tile': tile,
    },
    data: function () {
      return {
        newName: '',
        adding: false,
        selectedDictId: null,
        active: null,
        dialog: false,
        loading: false,
        loadingDictionaries: false,
        dictionaries: [],
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Added', value: 'time', align: 'right'},
          {text: 'Download', value: 'name', align: 'right'},
          {text: 'Delete', align: 'right'}
        ],
        dictionaryHeader: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Keyspace', value: 'keyspace', align: 'right'},
          {text: 'Time', value: 'time', align: 'right'},
          {text: 'Link to', value: 'name', sortable: false, align: 'right', width: "1"}
        ],
        markovChains: [],
      }
    },
    mounted: function () {
      this.loadMarkovChains()
    },
    methods: {
      loadMarkovChains: function () {
        this.dialog= false
        this.loading = true;
        this.axios.get(this.$serverAddr + '/markovChains', {}).then((response) => {
          this.markovChains = response.data;
          this.loading = false
        })
      },
      loadDictionaries: function () {
        this.loadingDictionaries = true;
        this.axios.get(this.$serverAddr + '/dictionary', {}).then((response) => {
          this.dictionaries = response.data;
          this.loadingDictionaries = false
        })
      },
      makeHcStatFromDictionary: function () {
        this.adding = true
        this.axios.post(this.$serverAddr + '/markovChains/makeFromDictionary', {
          "dictionary_id": this.selectedDictId,
          "name": this.newName
        }).then((response) => {
          this.adding = true
          this.dialog = false
          this.loadMarkovChains()
        }).catch((error) => {
          this.adding = false
        })
      },
      deleteMarkov: function (id) {
        this.$root.$confirm('Delete', 'Are you sure?').then((confirm) => {
          this.loading = true;
          this.axios.delete(this.$serverAddr + '/markovChains/' + id).then((response) => {
            this.loadMarkovChains()
          })
        })
      }
    }
  }
</script>

<style scoped>
  .noEvent {
    pointer-events: none;
    display: inline-block;
  }

  .dz-message {
    cursor: pointer;
    text-align: center;
  }

  .max500 {
    max-width: 600px;
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
</style>
