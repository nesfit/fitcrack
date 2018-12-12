<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max500">
    <fc-tile title="Encrypted files" class="ma-2" :loading="encryptedFiles === null">
      <v-data-table
        v-if="encryptedFiles !== null"
        :headers="headers"
        :items="encryptedFiles.items"
        :rows-per-page-items="[10,25,50]"
        rows-per-page-text="Files per page"
        disable-initial-sort
      >
        <template slot="items" slot-scope="props">
          <td>{{ props.item.name }}</td>
          <td class="text-xs-right">{{ props.item.hash }}</td>
          <td class="text-xs-right">{{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}</td>
          <td class="text-xs-right">
            <a :href="$serverAddr + '/protectedFiles/' + props.item.id" target="_blank">
            <v-btn outline fab small color="primary">
              <v-icon>file_download</v-icon>
            </v-btn>
            </a>
          </td>
        </template>
      </v-data-table>
      <vue-clip :options="uploadOptions" :on-total-progress="uploadProgressChanged" class="pa-2" :on-complete="uploadComplete">
        <template slot="clip-uploader-action">
          <div class="dz-message">
            <v-btn outline flat color="primary" class="noEvent">Upload file</v-btn>
          </div>
          <v-progress-linear background-color="white" v-model="uploadProgress"></v-progress-linear>
        </template>
        <template slot="clip-uploader-body" slot-scope="props">
          <div v-for="file in props.files">
            {{ file.name }} {{ file.status }}
          </div>
        </template>
      </vue-clip>
    </fc-tile>







  </v-container>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "encryptedFilesView",
    components: {
      'fc-tile': tile,
    },
    mounted: function () {
      this.loadFiles()
    },
    methods: {
      loadFiles: function () {
        this.axios.get(this.$serverAddr + '/protectedFiles', {}).then((response) => {
          this.encryptedFiles = response.data;
        })
      },
      uploadProgressChanged: function (progress, totalBytes, bytesSent) {
        this.uploadProgress = progress
      },

      uploadComplete: function (file, status, xhr) {
        if (status === 'error') {
          this.axios.interceptors.response.handlers["0"].rejected(JSON.parse(xhr.response))
        } else {
          this.loadFiles()
        }
      }
    },
    data: function () {
      return {
        uploadOptions: {
          url: this.$serverAddr + '/protectedFiles/',
          paramName: 'file'
        },
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Hash', value: 'hash', align: 'right'},
          {text: 'Added', value: 'time', align: 'right'},
          {text: 'Download', value: 'name', align: 'right'}
        ],
        encryptedFiles: [],
        uploadProgress: 0
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
