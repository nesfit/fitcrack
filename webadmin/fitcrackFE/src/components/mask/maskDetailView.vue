<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs divider="/" class="pb-0"  v-if="data != null">
      <v-breadcrumbs-item>
        <router-link :to="{name: 'masks'}">Masks</router-link>
      </v-breadcrumbs-item>
      <v-breadcrumbs-item>
        {{data.name}}
      </v-breadcrumbs-item>
    </v-breadcrumbs>

    <v-container>
      <v-layout row wrap justify-center>
        <fc-tile title="Mask set" :loading="data==null" class="mx-2 dictContentContainer mb-4">
          <v-list single-line class="width100" v-if="data != null">
            <v-list-tile class="px-2 py-1">
              <v-list-tile-action class="pr-3 key">
                Name:
              </v-list-tile-action>
              <v-list-tile-content>
                <v-list-tile-title class="text-xs-right">
                  {{data.name}}
                </v-list-tile-title>
              </v-list-tile-content>
            </v-list-tile>
            <v-divider></v-divider>
            <v-list-tile class="px-2 py-1">
              <v-list-tile-action class="pr-3 key">
                Added:
              </v-list-tile-action>
              <v-list-tile-content>
                <v-list-tile-title class="text-xs-right">
                  {{ $moment(data.time).format('DD.MM.YYYY HH:mm') }}
                </v-list-tile-title>
              </v-list-tile-content>
            </v-list-tile>
            <v-divider></v-divider>
            <v-list-tile class="px-2 py-1">
              <v-list-tile-content>
                <v-layout row class="width100 margintop5">
                  <v-btn class="ma-0" outline color="primary" v-if="!edit" @click="edit=true">
                    Edit <v-icon class="ml-2">edit</v-icon>
                  </v-btn>
                  <v-btn class="ma-0" outline color="primary" v-else @click="saveMask">
                    Save <v-icon class="ml-2">save</v-icon>
                  </v-btn>
                  <v-spacer></v-spacer>
                  <a :href="$serverAddr + '/masks/' + data.id + '/download'" target="_blank">
                    <v-btn class="ma-0" outline color="primary">
                      Download <v-icon class="ml-2">file_download</v-icon>
                    </v-btn>
                  </a>
                </v-layout>
              </v-list-tile-content>
            </v-list-tile>
          </v-list>
          <v-divider></v-divider>
          <div class="dictContent pa-2" v-if="data != null" v-bind:class="{editing: edit}">
            <code class="code elevation-0"
                  @input="update"
                  :contenteditable="edit"
            >{{data.data}}</code>
          </div>

        </fc-tile>
      </v-layout>
    </v-container>
  </div>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "maskDetailView",
    components: {
      'fc-tile': tile
    },
    mounted: function () {
      this.loadData()
    },
    methods: {
      loadData: function ($state) {
        this.axios.get(this.$serverAddr + '/masks/' + this.$route.params.id).then((response) => {
          this.data = response.data
        });
      },
      update:function(event){
        console.log('text updated')
        this.newData = event.target.innerText
      },
      saveMask: function () {
        if (this.newData === '')
          return

        this.axios.post(this.$serverAddr + '/masks/' + this.data.id + '/update', {
          "newMaskSet": this.newData
        }).then((response) => {
          console.log(response.data)
          this.edit=false
        })
      }
    },
    data: function () {
      return {
        data: null,
        edit: false,
        newData: ''
      }
    }
  }
</script>

<style scoped>
  .dictContentContainer {
    max-width: 800px;
  }
  .dictContent {
    overflow: auto;
    background: #eee;
  }

  .dictContent.editing {
    background: white;
  }

  .code::before {
    display: none;
  }
  .code {
    width: 100%;
    background: transparent;
    color: #000;
  }
  .width100 {
    width: 100%;
  }

  .margintop5 {
    margin-top: 10px;
  }
</style>
