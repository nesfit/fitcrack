<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max500">
    <fc-tile
      title="PCFGs"
      class="ma-2"
      :icon="$route.meta.icon"
    >
      <v-alert
        tile
        text
        type="warning"
        class="mb-0"
      >
        PCFG files must have a .zip extension.
      </v-alert>
      <v-data-table
        :headers="headers"
        :items="pcfg.items"
        :loading="loading"
        :footer-props="{itemsPerPageOptions: [10,25,50], itemsPerPageText: 'PCFGs per page'}"
      >
        <template v-slot:item.time="{ item }">
          {{ $moment(item.time).format('DD.MM.YYYY HH:mm') }}
        </template>
        <template v-slot:item.actions="{ item }">
          <v-tooltip top>
            <template v-slot:activator="{ on }">
              <a
                :href="$serverAddr + '/pcfg/' + item.id"
                target="_blank"
                download
                v-on="on"
              >
                <v-btn icon>
                  <v-icon>mdi-file-download-outline</v-icon>
                </v-btn>
              </a>
            </template>
            <span>Download</span>
          </v-tooltip>
          <v-tooltip top>
            <template v-slot:activator="{ on }">
              <v-btn
                icon
                @click="deletePcfg(item)"
                v-on="on"
              >
                <v-icon color="error">
                  mdi-delete-outline
                </v-icon>
              </v-btn>
            </template>
            <span>Delete</span>
          </v-tooltip>
        </template>
      </v-data-table>
      <div class="text-right pa-3">
        <v-btn
          class="d-inline-block"
          color="primary"
          text
          outlined
          @click.native.stop="dialog = true"
        >
          Add new
        </v-btn>
      </div>
    </fc-tile>

    <file-creator
      v-model="dialog"
      title="Add new PCFG grammar"
      :upload-url="this.$serverAddr + '/pcfg/add'"
      :working="working"
      @fileUploaded="loadPcfg"
      @dictionarySelected="makePcfgFromDictionary"
    />
  </v-container>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import fileCreator from "@/components/fileUploader/fileCreator"
  export default {
    name: "PcfgView",
    components: {
      fileCreator,
      'fc-tile': tile,
    },
    data: function () {
      return {
        working: false,
        dialog: false,
        loading: false,
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Added', value: 'time', align: 'end'},
          {text: 'Actions', value: 'actions', align: 'end', sortable: false}
        ],
        pcfg: []
      }
    },
    mounted: function () {
      this.loadPcfg()
    },
    methods: {
      loadPcfg: function () {
        this.loading = true
        this.axios.get(this.$serverAddr + '/pcfg', {}).then((response) => {
          this.pcfg = response.data;
          this.loading = false
        })
      },
      makePcfgFromDictionary: function (id, name) {
        this.working = true
        this.axios.post(this.$serverAddr + '/pcfg/makeFromDictionary', {
          "dictionary_id": id,
          "name": name
        }).then((response) => {
          this.working = false
          this.dialog = false
          this.loadPcfg()
        }).catch((error) => {
          this.working = false
        })
      },
      deletePcfg: function (item) {
        this.$root.$confirm('Delete', `This will remove ${item.name} from your PCFGs. Are you sure?`).then((confirm) => {
          this.loading = true
          this.axios.delete(this.$serverAddr + '/pcfg/' + item.id).then((response) => {
            this.loadPcfg()
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
