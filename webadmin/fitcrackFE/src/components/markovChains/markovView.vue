<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max500">
    <fc-tile title="Markov chains" class="ma-2">
      <v-alert :value="true" type="warning" class="mt-0 mb-1" >
        Markov files must have a .hcstat extension.
      </v-alert>
      <v-data-table
        :headers="headers"
        :items="markovChains.items"
        :loading="loading"
        :rows-per-page-items="[10,25,50]"
        rows-per-page-text="Hcstats per page"
        disable-initial-sort
      >
        <template slot="items" slot-scope="props">
          <td>{{ props.item.name }}</td>
          <td class="text-xs-right">{{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}</td>
          <td class="text-xs-right">
            <a :href="$serverAddr + '/markovChains/' + props.item.id" target="_blank">
            <v-btn outline fab small color="primary">
              <v-icon>file_download</v-icon>
            </v-btn>
            </a>
          </td>
          <td class="text-xs-right">
            <v-tooltip top>
              <v-btn icon class="mx-0" @click="deleteMarkov(props.item.id)" slot="activator">
                <v-icon color="error">delete</v-icon>
              </v-btn>
              <span>Delete Markov file</span>
            </v-tooltip>
          </td>
        </template>
      </v-data-table>
      <div class="text-xs-right pa-3">
        <v-btn class="d-inline-block" color="primary" flat outline @click.native.stop="dialog = true; loadDictionaries()">Add new</v-btn>
      </div>
    </fc-tile>








    <v-dialog v-model="dialog" max-width="600" lazy >
      <v-card>
        <v-card-title class="headline">Add new HcStat</v-card-title>
        <v-tabs
          v-model="active"
          color="grey lighten-4"
          light
          slider-color="primary">
          <v-tab ripple>
            Upload file
          </v-tab>
          <v-tab ripple>
            Make from dictionary
          </v-tab>
          <v-tab-item>
            <v-card flat>
              <file-uploader :url="this.$serverAddr + '/markovChains/add'" @uploadComplete="loadMarkovChains(); dialog=false"></file-uploader>
            </v-card>
          </v-tab-item>
          <v-tab-item>
            <div class="text-xs-center" v-if="adding">
              <v-progress-circular
                class="ma-5"
                size="50"
                :width="3"
                indeterminate
                color="primary">
              </v-progress-circular>
            </div>
            <v-card flat v-else>
              <div class="px-2">
                <v-text-field
                  name="name"
                  label="Name"
                  id="newHcStatName"
                  v-model="newName"
                ></v-text-field>
              </div>
              <v-data-table
                :headers="dictionaryHeader"
                :items="dictionaries.items"
                item-key="id"
                :loading="loadingDictionaries"
                :rows-per-page-items="[10,25,50]"
                rows-per-page-text="Dictionaries per page"
                disable-initial-sort
              >
                <template slot="items" slot-scope="props">
                  <tr @click="selectedDictId=props.item.id" class="clickable"
                      v-bind:class="{selectedDict: (props.item.id === selectedDictId)}" >
                    <td>{{ props.item.name }}</td>
                    <td class="text-xs-right">{{ props.item.keyspace }}</td>
                    <td class="text-xs-right">{{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}</td>
                    <td class="text-xs-right">
                      <v-tooltip top>
                        <v-btn icon class="mx-0" :to="{name: 'dictionaryDetail', params: { id: props.item.id}}" slot="activator">
                          <v-icon color="primary">link</v-icon>
                        </v-btn>
                        <span>Go to the dictionary page</span>
                      </v-tooltip>
                    </td>
                  </tr>

                </template>
              </v-data-table>
              <div class="text-xs-right pa-3">
                <v-btn class="d-inline-block"
                       color="primary"
                       flat
                       outline
                       @click="makeHcStatFromDictionary"
                       :disabled="selectedDictId === null || newName === ''">
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
    name: "markovView",
    components: {
      FileUploader,
      'fc-tile': tile,
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
          this.adding = false
          this.dialog = false
          this.loadMarkovChains()
        }).catch((error) => {
          this.adding = false
        })
      },
      deleteMarkov: function (id) {
        this.$root.$confirm('Delete', 'Are you sure?', { color: 'primary' }).then((confirm) => {
          this.loading = true;
          this.axios.delete(this.$serverAddr + '/markovChains/' + id).then((response) => {
            this.loadMarkovChains()
          })
        })
      }
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
