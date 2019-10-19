
<template>
  <v-container fluid>
    <v-layout column align-center>
      <div class="container max1000">
        <v-layout row wrap>
          <fc-tile title="Jobs and hosts" class="mx-3 mb-5 height100 wid20" :loading="jobsInfo === null">
            <v-list single-line class="width100" v-if="jobsInfo != null">
              <template v-for="(item, i) in jobsInfo">
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    {{item.status}}:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-xs-right">
                      {{item.count}}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider v-if="!(jobsInfo.length === i+1)"></v-divider>
              </template>
              <v-divider></v-divider>
              <v-divider></v-divider>

              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key">
                  online hosts:
                </v-list-item-action>
                <v-list-item-content>
                  <v-list-item-title class="text-xs-right">
                    {{hostsInfo.activeHosts}}
                  </v-list-item-title>
                </v-list-item-content>
              </v-list-item>
              <v-divider></v-divider>
              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key">
                  offline hosts:
                </v-list-item-action>
                <v-list-item-content>
                  <v-list-item-title class="text-xs-right">
                    {{hostsInfo.inactiveHosts}}
                  </v-list-item-title>
                </v-list-item-content>
              </v-list-item>
            </v-list>
          </fc-tile>
          <fc-tile title="Last active jobs" class="mx-3 mb-5 height100 wid20" :loading="lastJobs === null">
            <v-list single-line class="width100" v-if="lastJobs != null">
              <template v-for="(item, i) in lastJobs">
                <v-list-item class="px-2 py-1" :to="'/jobs/' + item.id">
                  <v-list-item-action class="pr-3 key">
                    {{item.name}}
                  </v-list-item-action>
                  <v-list-item-content>
                    {{progressToPercentage(item.progress)}}
                  </v-list-item-content>

                  <v-list-item-content>
                    <v-list-item-title v-bind:class="item.status_type + '--text'"
                                       class="text-xs-right fw500">
                      <v-tooltip top>
                          <template v-slot:activator="{ on }">
                            <span v-on="on">
                            {{ item.status_text }}
                            </span>
                          </template>
                        <span>{{ item.status_tooltip }}</span>
                      </v-tooltip>
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider v-if="!(lastJobs.length === i+1)"></v-divider>
              </template>
            </v-list>
          </fc-tile>
          <fc-tile title="Server" class="mx-3 mb-5 height100 wid20" :loading="actualUsage == null">
            <v-list single-line class="width100" v-if="actualUsage != null">
              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key" v-text="'CPU / RAM'"/>
                <v-list-item-content>
                  <v-list-item-title class="text-xs-right"
                                     v-text="actualUsage.cpu + ' % / ' + actualUsage.ram + ' %'"/>
                </v-list-item-content>
              </v-list-item>
              <v-divider/>

              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key" v-text="'HDD read'"/>
                <v-list-item-content>
                  <v-list-item-title class="text-xs-right" v-text="actualUsage.hdd_read + ' kb/s'"/>
                </v-list-item-content>
              </v-list-item>
              <v-divider/>

              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key" v-text="'HDD write'"/>
                <v-list-item-content>
                  <v-list-item-title class="text-xs-right" v-text="actualUsage.hdd_write + ' kb/s'"/>
                </v-list-item-content>
              </v-list-item>
              <v-divider/>

              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key" v-text="'Download'"/>
                <v-list-item-content>
                  <v-list-item-title class="text-xs-right" v-text="actualUsage.net_recv + ' kb/s'"/>
                </v-list-item-content>
              </v-list-item>
              <v-divider/>

              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key" v-text="'Upload'"/>
                <v-list-item-content>
                  <v-list-item-title class="text-xs-right" v-text="actualUsage.net_sent + ' kb/s'"/>
                </v-list-item-content>
              </v-list-item>
              <v-divider/>

              <v-list-item class="px-2 py-1">
                <v-list-item-action class="pr-3 key" v-text="'Services up/down'"/>
                <v-list-item-content>
                  <v-list-item-title class="text-xs-right"
                                     v-text="serverInfoUp + ' / ' + serverInfoDown"/>
                </v-list-item-content>
              </v-list-item>


            </v-list>
          </fc-tile>
          <fc-tile title="Notifications" class="mx-3 mb-5 height100 wid20" :loading="notifications == null">
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
        </v-layout>
        <v-layout row>
          <fc-tile title="Jobs progress" class="mx-3 mb-5 height100 flex1">
            <fc-graph
                    id="packageGraph"
                    :data='packageGraph'
                    type="job"
            >
            </fc-graph>
          </fc-tile>
          <fc-tile title="Hosts activity" class="mx-3 height100 flex1">
            <fc-graph
                    id="hostGraph"
                    :data='hostGraph'
                    type="host"
            >
            </fc-graph>
          </fc-tile>
        </v-layout>

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
                        text
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
                        text
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
        serverInfo: null,
        serverInfoUp: 1,
        serverInfoDown: 1,
        lastJobs: null,
        actualUsage: {
          cpu: 0,
          ram: 0,
          hdd_read: 0,
          hdd_write: 0,
          net_recv: 0,
          net_sent: 0
        }
      }
    },
    methods: {
      yesNo: function (val) {
        return val ? 'Yes' : 'No'
      },
      progressToPercentage: function (progress) {
        return progress.toFixed() + '%'
      },
      loadData: function () {


        this.axios.get(this.$serverAddr + '/jobs/lastJobs')
                .then((response) => {
                  this.lastJobs = response.data.items;
                });

        this.axios.get(this.$serverAddr + '/serverInfo/actualUsageData'
        ).then((response) => {
          this.actualUsage = response.data;
        });


        var fromDate = '';
        var toDate = '';

        if (this.fullDate) {
          if (this.fromDate.length === 12) {
            fromDate = this.$moment(this.fromDate, 'DDMMYYYYHHmm').format('YYYY-M-D H:mm:ss');
          } else {
            return
          }
          if (this.toDate.length === 12) {
            toDate = this.$moment(this.toDate, 'DDMMYYYYHHmm').format('YYYY-M-D H:mm:ss');
          } else {
            return
          }
        } else {
          fromDate = this.$moment().subtract(this.hoursBefore, 'hours').format('YYYY-M-D H:mm:ss');
        }


        this.axios.get(this.$serverAddr + '/graph/packagesProgress?from_date=' + fromDate + '&to_date=' + toDate).then((response) => {
          this.packageGraph = response.data;
        });
        this.axios.get(this.$serverAddr + '/graph/hostsComputing?from_date=' + fromDate + '&to_date=' + toDate).then((response) => {
          this.hostGraph = response.data;
        });

        this.axios.get(this.$serverAddr + '/notifications?seen=false').then((response) => {
          this.notifications = response.data;
        });

        this.axios.get(this.$serverAddr + '/hosts/info').then((response) => {
          this.hostsInfo = response.data;
        });

        this.axios.get(this.$serverAddr + '/jobs/info').then((response) => {
          this.jobsInfo = response.data;
        });

        this.axios.get(this.$serverAddr + '/serverInfo/info').then((response) => {
          this.serverInfo = response.data.subsystems;

          const statuses = this.serverInfo.map(a => a.status);
          this.serverInfoUp = 0;
          this.serverInfoDown = 0;

          for (let i = 0; i < statuses.length; i++) {
            if (statuses[i] === "running")
              this.serverInfoUp++;
            else
              this.serverInfoDown++;
          }
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

  .dateSelect {
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
    max-width: 3000px;
  }

  .key {
    font-weight: bold;
  }

  .height100 {
    height: 100%;
  }

  .flex1 {
    flex: 1;
  }

  .wid20 {
    width: 20%;
  }
</style>
