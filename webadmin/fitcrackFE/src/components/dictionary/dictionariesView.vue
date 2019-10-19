<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max500">
    <fc-tile
      title="Dictionaries"
      class="ma-2"
    >
      <v-alert
        :value="true"
        type="warning"
        class="mt-2 mb-0"
      >
        Dictionaries must have a .txt extension. The preferred way to upload dictionaries is through SFTP server.
      </v-alert>
      <v-data-table
        :headers="headers"
        :items="dictionaries.items"
        :loading="loading"
        :footer-props="{itemsPerPageOptions: [10,25,50], itemsPerPageText: 'Dictionaries per page'}"
      >
        <template
          slot="items"
          slot-scope="props"
        >
          <td>
            <router-link :to="{name: 'dictionaryDetail', params: { id: props.item.id}}">
              {{ props.item.name }}
            </router-link>
          </td>
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
                  @click="deleteDictionary(props.item.id)"
                  v-on="on"
                >
                  <v-icon color="error">
                    delete
                  </v-icon>
                </v-btn>
              </template>
              <span>Delete dictionary</span>
            </v-tooltip>
          </td>
        </template>
      </v-data-table>
      <v-divider />
      <div class="text-right px-2 ">
        <v-btn
          class="d-inline-block"
          color="primary"
          text
          outlined
          @click.native.stop="dialog = true"
        >
          Select from drive
        </v-btn>
      </div>

      <file-uploader
        :url="this.$serverAddr + '/dictionary/add'"
        @uploadComplete="loadDictionaries"
      />
    </fc-tile>

    <v-dialog
      v-model="dialog"
      max-width="500"
    >
      <server-browser @filesuploaded="dialog = false;loadDictionaries()" />
    </v-dialog>
  </v-container>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import serverBrowser from '@/components/serverBrowser/serverBrowser'
  import FileUploader from "@/components/fileUploader/fileUploader";

  export default {
    name: "DictionariesView",
    components: {
      FileUploader,
      'fc-tile': tile,
      'server-browser': serverBrowser
    },
    data: function () {
      return {
        loading: false,
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Keyspace', value: 'keyspace', align: 'right'},
          {text: 'Time', value: 'time', align: 'right'},
          {text: 'Delete', align: 'right'}
        ],
        dictionaries: [],
        dialog: false
      }
    },
    mounted: function () {
      this.loadDictionaries()
    },
    methods: {
      loadDictionaries: function () {
        this.loading = true;
        this.axios.get(this.$serverAddr + '/dictionary', {}).then((response) => {
          this.dictionaries = response.data;
          this.loading = false
        })
      },
      deleteDictionary: function (id) {
        this.$root.$confirm('Delete', 'Are you sure?', { color: 'primary' }).then((confirm) => {
          this.loading = true;
          this.axios.delete(this.$serverAddr + '/dictionary/' + id).then((response) => {
            this.loadDictionaries()
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
    text-align: right;
  }

  .max500 {
    max-width: 550px;
  }

</style>
