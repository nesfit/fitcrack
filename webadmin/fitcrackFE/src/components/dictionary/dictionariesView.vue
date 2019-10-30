<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max500">
    <fc-tile
      title="Dictionaries"
      :icon="$route.meta.icon"
      class="ma-2"
    >
      <v-alert
        tile
        text
        type="warning"
        class="mb-0"
      >
        Dictionaries must have a .txt extension. The preferred way to upload dictionaries is via SFTP.
      </v-alert>
      <v-data-table
        :headers="headers"
        :items="dictionaries.items"
        :loading="loading"
        :footer-props="{itemsPerPageOptions: [10,25,50], itemsPerPageText: 'Dictionaries per page'}"
      >
        <template v-slot:item.name="{ item }">
          <router-link :to="`dictionaries/${item.id}`">
            {{ item.name }}
          </router-link>
        </template>
        <template v-slot:item.time="{ item }">
          {{ $moment(item.time ).format('DD.MM.YYYY HH:mm') }}
        </template>
        <template v-slot:item.actions="{ item }">
          <v-tooltip top>
            <template v-slot:activator="{ on }">
              <v-btn
                icon
                class="mx-0"
                @click="deleteDictionary(item)"
                v-on="on"
              >
                <v-icon color="error">
                  mdi-delete-outline
                </v-icon>
              </v-btn>
            </template>
            <span>Delete dictionary</span>
          </v-tooltip>
        </template>
      </v-data-table>
      <v-btn
        color="primary"
        block
        text
        @click.native.stop="dialog = true"
      >
        Select from server
      </v-btn>
      <v-divider />

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
            align: 'start',
            value: 'name'
          },
          {text: 'Keyspace', value: 'keyspace', align: 'end'},
          {text: 'Time', value: 'time', align: 'end'},
          {text: 'Delete', value: 'actions', align: 'end', sortable: false}
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
      deleteDictionary: function (item) {
        this.$root.$confirm('Delete', `This will remove ${item.name} from your dictionaries. Are you sure?`).then((confirm) => {
          this.loading = true;
          this.axios.delete(this.$serverAddr + '/dictionary/' + item.id).then((response) => {
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
