<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max500">
    <fc-tile
      title="Charsets"
      class="ma-2"
    >
      <v-alert
        :value="true"
        type="warning"
        class="mt-0 mb-1"
      >
        Charsets must have a .txt, .hcchr or .charset extension.
      </v-alert>
      <v-data-table
        :headers="headers"
        :items="charsets.items"
        :loading="loading"
        :footer-props="{itemsPerPageOptions: [10,25,50], itemsPerPageText: 'Charsets per page'}"
      >
        <template
          slot="items"
          slot-scope="props"
        >
          <td>
            <router-link :to="{name: 'charsetDetail', params: { id: props.item.id}}">
              {{ props.item.name }}
            </router-link>
          </td>
          <td class="text-right">
            {{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}
          </td>
          <td class="text-right">
            <a
              :href="$serverAddr + '/charset/' + props.item.id + '/download'"
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
                  @click="deleteCharset(props.item.id)"
                  v-on="on"
                >
                  <v-icon color="error">
                    delete
                  </v-icon>
                </v-btn>
              </template>
              <span>Delete charset file</span>
            </v-tooltip>
          </td>
        </template>
      </v-data-table>
      <v-divider />
      <file-uploader
        :url="this.$serverAddr + '/charset/add'"
        @uploadComplete="loadCharsets"
      />
    </fc-tile>
  </v-container>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import FileUploader from "@/components/fileUploader/fileUploader";
  export default {
    name: "CharsetView",
    components: {
      FileUploader,
      'fc-tile': tile,
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
