<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-container>
      <fc-tile title="Server" class="mx-auto mt-5 max500" :loading="serverInfo == null || loading">
        <v-list single-line class="width100" v-if="serverInfo != null">
          <template v-for="(value, key, i) in serverInfo.subsystems">
            <v-list-tile class="px-2 py-1">
              <v-list-tile-action class="pr-3 key">
                {{key}}:
              </v-list-tile-action>
              <v-list-tile-content>
                <v-list-tile-title class="text-xs-right error--text" v-bind:class="{'success--text': value}">
                  {{ yesNo(value) }}
                </v-list-tile-title>
              </v-list-tile-content>
            </v-list-tile>
            <v-divider></v-divider>
          </template>
          <v-list-tile class="px-2 py-1">
            <v-list-tile-content class="">
              <v-list-tile-title class="height100 text-xs-center">
                <v-btn @click="operation('start')" flat outline color="success">Start</v-btn>
                <v-btn @click="operation('stop')" flat outline color="error">Stop</v-btn>
                <v-btn @click="operation('restart')" flat outline color="info">Restart</v-btn>
              </v-list-tile-title>
            </v-list-tile-content>
          </v-list-tile>
        </v-list>
      </fc-tile>
    </v-container>

</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "serverPage",
    components: {
      'fc-tile': tile,
    },
    mounted: function() {
      this.loadData()
    },
    methods: {
      loadData: function () {
        this.axios.get(this.$serverAddr + '/serverInfo/info').then((response) => {
          this.serverInfo = response.data
        })
      },
      operation: function (oper) {
        this.loading = true
        this.axios.get(this.$serverAddr + '/serverInfo/control?operation=' + oper).then((response) => {
          this.loadData()
          this.loading = false
        }).catch((error) => {
          this.loading = false
        })
      },
      yesNo: function (val) {
        return val ? 'Yes' : 'No'
      }
    },
    data: function () {
      return {
        serverInfo: null,
        loading: false
      }
    }
  }
</script>

<style scoped>
  .max500 {
    max-width: 500px;
  }

  .height100 {
    height: 100%;
  }
</style>
