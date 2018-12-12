<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max500">
    <fc-tile title="Charsets" class="ma-2">
      <v-alert :value="true" type="warning" class="mt-0 mb-1" >
        Charsets must have a .txt, .hcchr or .charset extension.
      </v-alert>
      <v-data-table
        :headers="headers"
        :items="charsets.items"
        :loading="loading"
        :rows-per-page-items="[10,25,50]"
        rows-per-page-text="Charsets sets per page"
        disable-initial-sort
      >
        <template slot="items" slot-scope="props">
          <td>
            <router-link :to="{name: 'charsetDetail', params: { id: props.item.id}}">{{ props.item.name }}</router-link>
          </td>
          <td class="text-xs-right">{{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}</td>
          <td class="text-xs-right">
            <a :href="$serverAddr + '/charset/' + props.item.id + '/download'" target="_blank">
              <v-btn outline fab small color="primary">
                <v-icon>file_download</v-icon>
              </v-btn>
            </a>
          </td>
          <td class="text-xs-right">
            <v-tooltip top>
              <v-btn icon class="mx-0" @click="deleteCharset(props.item.id)" slot="activator">
                <v-icon color="error">delete</v-icon>
              </v-btn>
              <span>Delete charset file</span>
            </v-tooltip>
          </td>
        </template>
      </v-data-table>
      <v-divider></v-divider>
      <file-uploader :url="this.$serverAddr + '/charset/add'" @uploadComplete="loadCharsets"></file-uploader>

    </fc-tile>

  </v-container>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import FileUploader from "@/components/fileUploader/fileUploader";
  export default {
    name: "charsetView",
    components: {
      FileUploader,
      'fc-tile': tile,
    },
    mounted: function () {
      this.loadCharsets()
    },
    methods: {
      loadCharsets: function () {
        this.loading = true;
        this.axios.get(this.$serverAddr + '/charset', {}).then((response) => {
          this.charsets = response.data;
          this.loading = false
        })
      },
      deleteCharset: function (id) {
        this.$root.$confirm('Delete', 'Are you sure?', { color: 'primary' }).then((confirm) => {
          this.loading = true;
          this.axios.delete(this.$serverAddr + '/charset/' + id).then((response) => {
            this.loadCharsets()
          })
        })
      }
    },
    data: function () {
      return {
        loading: false,
        charsets: [],
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Added', value: 'time', align: 'right'},
          {text: 'Download', value: 'name', align: 'right'},
          {text: 'Delete', align: 'right'}
        ]
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
