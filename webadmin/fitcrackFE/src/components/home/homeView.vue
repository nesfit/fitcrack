<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container fluid>
      <v-layout column align-center>
        <div class="container max1000">
          <v-layout row wrap>

            <fc-tile title="Jobs" class="mx-3 mb-5 height100" :loading="jobsInfo === null">
              <v-list single-line class="width100" v-if="jobsInfo != null">
                <template v-for="(item, i) in jobsInfo">
                  <v-list-tile class="px-2 py-1">
                    <v-list-tile-action class="pr-3 key">
                      {{item.status}}:
                    </v-list-tile-action>
                    <v-list-tile-content>
                      <v-list-tile-title class="text-xs-right">
                        {{item.count}}
                      </v-list-tile-title>
                    </v-list-tile-content>
                  </v-list-tile>
                  <v-divider v-if="!(jobsInfo.length === i+1)"></v-divider>
                </template>
              </v-list>
            </fc-tile>


            <fc-tile title="Hosts" class="mx-3 mb-5 height100" :loading="hostsInfo === null">
              <v-list single-line class="width100" v-if="hostsInfo != null">
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Total hosts:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">
                      {{hostsInfo.totalHosts}}
                    </v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Online hosts:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">
                      {{hostsInfo.activeHosts}}
                    </v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Offline hosts:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">
                      {{hostsInfo.inactiveHosts}}
                    </v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
              </v-list>
            </fc-tile>


            <fc-tile title="Notifications" class="mx-3 mb-5 height100" :loading="notifications == null">
              <div v-if="notifications !== null" class="notifCont">
                <v-list class="notifContainer pa-0"
                        v-for="(item, i) in notifications.items"
                        :key="i"
                >
                  <notification :type="item.type"
                                :text="item.text"
                                :seen="item.seen"
                                :time="item.time"
                                :job-id="item.job_id"
                  ></notification>
                </v-list>
              </div>
            </fc-tile>


            <fc-tile title="Server" class="mx-3 mb-5 height100" :loading="serverInfo == null">
              <v-list single-line class="width100" v-if="serverInfo != null">
                <template v-for="(subsystem, i) in serverInfo.subsystems">
                  <v-list-tile class="px-2 py-1">
                    <v-list-tile-action class="pr-3 key">
                      {{subsystem.name}}:
                    </v-list-tile-action>
                    <v-list-tile-content>
                      <v-list-tile-title class="text-xs-right error--text" v-bind:class="{'success--text': subsystem.status==='running'}">
                        {{ subsystem.status }}
                      </v-list-tile-title>
                    </v-list-tile-content>
                  </v-list-tile>
                  <v-divider v-if="!(Object.keys(serverInfo.subsystems).length === i+1)"></v-divider>
                </template>
              </v-list>
            </fc-tile>

          </v-layout>

          <fc-tile title="Jobs progress" class="mx-3 mb-5">
            <fc-graph
              id="packageGraph"
              :data='packageGraph'
              type="job"
            >
            </fc-graph>
          </fc-tile>

          <fc-tile title="Hosts activity" class="mx-3">
            <fc-graph
              id="hostGraph"
              :data='hostGraph'
              type="host"
            >
            </fc-graph>
          </fc-tile>
          <v-layout class=" dateSelectCont">
            <v-spacer></v-spacer>
            <v-flex xs5 @click="fullDate = false">
              <v-radio-group row class="dateSelect" v-model="hoursBefore" @change="loadData">
                <v-radio value="1" label="last hour" :disabled="fullDate"></v-radio>
                <v-radio value="3" label="last 3 hours" :disabled="fullDate"></v-radio>
                <v-radio value="6" label="last 6 hours" :disabled="fullDate"></v-radio>
              </v-radio-group>
            </v-flex>
            <v-flex xs6 @click="fullDate = true" class="pl-2">
              <v-layout row>
                <v-flex xs2>
                  <v-subheader class="textBottom">From:</v-subheader>
                </v-flex>
                <v-flex xs4>
                  <v-text-field
                    @input="loadData"
                    :disabled="!fullDate"
                    v-model="fromDate"
                    flat
                    single-line
                    mask="date-with-time"
                    :placeholder="this.$moment().format('DD/MM/YYYY HH:MM')"
                  ></v-text-field>
                </v-flex>
                <v-flex xs2>
                  <v-subheader class="textBottom">To:</v-subheader>
                </v-flex>
                <v-flex xs4>
                  <v-text-field
                    @input="loadData"
                    :disabled="!fullDate"
                    v-model="toDate"
                    flat
                    single-line
                    mask="date-with-time"
                    :placeholder="this.$moment().format('DD/MM/YYYY HH:MM')"
                  ></v-text-field>
                </v-flex>
              </v-layout>

            </v-flex>
          </v-layout>
        </div>

      </v-layout>
  </v-container>
</template>

<script>
  import graph from '@/components/graph/fc_graph'
  import tile from '@/components/tile/fc_tile'
  import notification from '@/components/notification/fc_notification'

  export default {
    name: 'homeView',
    components: {
      'fc-graph': graph,
      'fc-tile': tile,
      'notification': notification
    },
    mounted: function () {
      this.loadData();

      this.interval = setInterval(function () {
        this.loadData()
      }.bind(this), 15000)
    },
    beforeDestroy: function () {
      clearInterval(this.interval)
    },
    data: function () {
      return {
        packageGraph: null,
        hostGraph: null,
        interval: null,
        hoursBefore: '6',
        fromDate: this.$moment().format('DDMMYYYYHHmm'),
        toDate: this.$moment().format('DDMMYYYYHHmm'),
        fullDate: false,
        notifications: null,
        hostsInfo: null,
        jobsInfo: null,
        serverInfo: null
      }
    },
    methods: {
      yesNo: function (val) {
        return val ? 'Yes' : 'No'
      },
      loadData: function () {
        var fromDate = ''
        var toDate = ''

        if (this.fullDate) {
          if (this.fromDate.length == 12) {
            fromDate = this.$moment(this.fromDate, 'DDMMYYYYHHmm').format('YYYY-M-D H:mm:ss');
          } else {
            return
          }
          if (this.toDate.length == 12) {
            toDate = this.$moment(this.toDate, 'DDMMYYYYHHmm').format('YYYY-M-D H:mm:ss');
          } else {
            return
          }
        } else {
          fromDate = this.$moment().subtract(this.hoursBefore, 'hours').format('YYYY-M-D H:mm:ss');
        }

        this.axios.get(this.$serverAddr + '/graph/packagesProgress?from_date=' + fromDate + '&to_date=' + toDate).then((response) => {
          this.packageGraph = response.data
        });
        this.axios.get(this.$serverAddr + '/graph/hostsComputing?from_date=' + fromDate + '&to_date=' + toDate).then((response) => {
          this.hostGraph = response.data
        });

        this.axios.get(this.$serverAddr + '/notifications?seen=false').then((response) => {
          this.notifications = response.data
        });

        this.axios.get(this.$serverAddr + '/hosts/info').then((response) => {
          this.hostsInfo = response.data
        });

        this.axios.get(this.$serverAddr + '/jobs/info').then((response) => {
          this.jobsInfo = response.data
        });

        this.axios.get(this.$serverAddr + '/serverInfo/info').then((response) => {
          this.serverInfo = response.data
        })
      }
    }
  }
</script>

<style scoped>
  .textBottom {
    align-items: flex-end;
    justify-content: flex-end;
  }

  .dateSelectCont {
    width: 100%;
    text-align: right;
  }

  .graph {
    width: 100%;
  }

  .dateSelect{
    display: inline-block;
    max-width: 450px;
  }
  .container {
    width: 100%;
    text-align: center;
  }

  .notifCont {
    width: 100%;
    max-height: 400px;
    overflow: auto;
  }

  .max1000 {
    max-width: 1000px;
  }

  .key {
    font-weight: bold;
  }

  .height100 {
    height: 100%;
  }


</style>
