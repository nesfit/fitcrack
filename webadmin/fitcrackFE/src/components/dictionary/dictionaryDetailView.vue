<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs divider="/" class="pb-0"  v-if="info != null">
      <v-breadcrumbs-item>
        <router-link :to="{name: 'dictionaries'}">Dictionaries</router-link>
      </v-breadcrumbs-item>
      <v-breadcrumbs-item>
        {{info.name}}
      </v-breadcrumbs-item>
    </v-breadcrumbs>

    <v-container>
      <v-layout row wrap justify-center>
        <fc-tile title="Dictionary" :loading="info==null" class="mx-2 dictContentContainer mb-4">
          <v-list single-line class="width100" v-if="info != null">
            <v-list-item class="px-2 py-1">
              <v-list-item-action class="pr-3 key">
                Name:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-xs-right">
                  {{info.name}}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
            <v-divider></v-divider>
            <v-list-item class="px-2 py-1">
              <v-list-item-action class="pr-3 key">
                Keyspace:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-xs-right">
                  {{info.keyspace}}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
            <v-divider></v-divider>
            <v-list-item class="px-2 py-1">
              <v-list-item-action class="pr-3 key">
                Added:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-xs-right">
                  {{ $moment(info.time).format('DD.MM.YYYY HH:mm') }}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
          </v-list>
          <v-divider></v-divider>
          <div class="dictContent" v-if="info != null">
            <fc-textarea :searchEnable="true" :readonly="true" :url="this.$serverAddr + '/dictionary/' +  this.$route.params.id + '/data'" maxHeight="500"></fc-textarea>
          </div>
        </fc-tile>
      </v-layout>
    </v-container>
  </div>
</template>

<script>
  import fcTextarea from '@/components/textarea/fc_textarea'
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "dictionaryView",
    components: {
      'fc-tile': tile,
      'fc-textarea': fcTextarea,
    },
    mounted: function () {
      this.loadData()
    },
    methods: {
      loadData: function ($state) {
        this.axios.get(this.$serverAddr + '/dictionary/' + this.$route.params.id).then((response) => {
            this.info = response.data
        });
      }
    },
    data: function () {
      return {
        info: null
      }
    }
  }
</script>

<style scoped>
  .dictContentContainer {
    max-width: 500px;
  }
  .dictContent {
    max-height: 600px;
  }

</style>
