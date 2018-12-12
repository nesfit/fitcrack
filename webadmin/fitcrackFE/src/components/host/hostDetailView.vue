<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs divider="/" class="pb-0"  v-if="data != null">
      <v-breadcrumbs-item>
        <router-link :to="{name: 'hosts'}">Hosts</router-link>
      </v-breadcrumbs-item>
      <v-breadcrumbs-item>
        {{data.domain_name}}
      </v-breadcrumbs-item>
    </v-breadcrumbs>

    <v-container>


        <fc-tile title="Host info" :loading="data==null" class=" dictContentContainer mb-5">
          <v-list single-line class="width100" v-if="data != null">
            <v-list-tile class="px-2 py-1">
              <v-list-tile-action class="pr-3 key">
                Name:
              </v-list-tile-action>
              <v-list-tile-content>
                <v-list-tile-title class="text-xs-right">
                  {{data.domain_name}}
                </v-list-tile-title>
              </v-list-tile-content>
            </v-list-tile>
            <v-divider></v-divider>
            <v-list-tile class="px-2 py-1">
              <v-list-tile-action class="pr-3 key">
                User:
              </v-list-tile-action>
              <v-list-tile-content>
                <v-list-tile-title class="text-xs-right">
                  {{data.user.name}}
                </v-list-tile-title>
              </v-list-tile-content>
            </v-list-tile>
            <v-divider></v-divider>
            <v-list-tile class="px-2 py-1">
              <v-list-tile-action class="pr-3 key">
                Operation system:
              </v-list-tile-action>
              <v-list-tile-content>
                <v-list-tile-title class="text-xs-right">
                  {{ data.os_name }}
                </v-list-tile-title>
              </v-list-tile-content>
            </v-list-tile>
            <v-divider></v-divider>
            <v-list-tile class="px-2 py-1">
              <v-list-tile-action class="pr-3 key">
                Processor:
              </v-list-tile-action>
              <v-list-tile-content>
                <v-list-tile-title class="text-xs-right">
                  {{ data.p_model }}
                </v-list-tile-title>
              </v-list-tile-content>
            </v-list-tile>
          </v-list>
          <v-divider></v-divider>
        </fc-tile>


        <fc-tile title="Jobs" class="max800 mb-5" :loading="data === null">
          <v-data-table
            v-if="data !== null"
            :headers="jobHeaders"
            :items="data.jobs"
            :rows-per-page-items="[10, 25,50,100]"
            rows-per-page-text="Jobs per page"
            disable-initial-sort
          >
            <template slot="items" slot-scope="props">
              <router-link :to="{ name: 'jobDetail', params: { id: props.item.id}}" class="middle">
                <td>{{ props.item.name }}</td>
              </router-link>
              <td class="text-xs-right">{{ props.item.attack }}</td>
              <td class="text-xs-right" v-bind:class="props.item.status_type + '--text'">{{ props.item.status_text }}</td>
              <td class="text-xs-right">
                <v-progress-circular
                  size="35"
                  :width="1.5"
                  :rotate="270"
                  color="primary"
                  class="jobProgress"
                  :value="props.item.progress"
                >
                  <span class="progressPercentage">{{ progressToPercentage(props.item.progress) }}</span>
                </v-progress-circular>
              </td>
              <td class="text-xs-right">{{ $moment(props.item.time).format('D.M.YYYY H:mm:ss') }}</td>
              <td class="text-xs-right">{{ props.item.password }}</td>
              <td>
                <div class="d-flex text-xs-right actionsBtns">
                  <v-tooltip top>
                    <v-btn icon class="mx-0"  :disabled=" props.item.status !== '0'"  slot="activator" @click="operateJob(props.item.id, 'start')">
                      <v-icon color="success">play_circle_outline</v-icon>
                    </v-btn>
                    <span>Start job</span>
                  </v-tooltip>
                  <v-tooltip top>
                    <v-btn icon class="mx-0"  :disabled=" props.item.status >= 10"  slot="activator" @click="operateJob(props.item.id, 'restart')">
                      <v-icon color="info">loop</v-icon>
                    </v-btn>
                    <span>Restart job</span>
                  </v-tooltip>
                  <v-tooltip top>
                    <v-btn icon class="mx-0"  :disabled=" props.item.status !== '10'"  slot="activator" @click="operateJob(props.item.id, 'stop')">
                      <v-icon color="error">pause_circle_outline</v-icon>
                    </v-btn>
                    <span>Stop job</span>
                  </v-tooltip>
                </div>
              </td>

            </template>
          </v-data-table>
        </fc-tile>



        <fc-tile title="Workunits" class="max800 mb-5" :loading="data === null">
          <v-data-table
            v-if="data !== null"
            :rows-per-page-items="[10,30,60,{'text':'All','value':-1}]"
            rows-per-page-text="Workunits per page"
            disable-initial-sort
            :headers="workunitsHeader"
            :items="data.workunits"
            class="width100"
          >
            <template slot="items" slot-scope="props">
              <router-link :to="{ name: 'jobDetail', params: { id: props.item.package_id}}" class="middle">
                <td>{{ props.item.job.name }}</td>
              </router-link>
              <td>{{ props.item.cracking_time_str }}</td>
              <td class="text-xs-right">{{ $moment(props.item.time).format('D.M.YYYY H:mm:ss') }}</td>
              <td class="text-xs-right">{{ props.item.start_index }}</td>
              <td class="text-xs-right">{{ props.item.hc_keyspace }}</td>
              <td class="text-xs-right error--text" v-bind:class="{'success--text': props.item.retry}">{{
                yesNo(props.item.retry) }}
              </td>
              <td class="text-xs-right error--text" v-bind:class="{'success--text': props.item.finished}">{{
                yesNo(props.item.finished) }}
              </td>
            </template>
          </v-data-table>
        </fc-tile>

    </v-container>
  </div>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "hostDetailView",
    components: {
      'fc-tile': tile
    },
    mounted: function () {
      this.loadData()
    },
    methods: {
      loadData: function () {
        this.axios.get(this.$serverAddr + '/hosts/' + this.$route.params.id).then((response) => {
          this.data = response.data;
        })
      },
      yesNo: function (val) {
        return val ? 'Yes' : 'No'
      },
      progressToPercentage: function (progress) {
        return progress.toFixed() + '%'
      },
      operateJob: function (id, operation) {
        this.axios.get(this.$serverAddr + '/jobs/' + id + '/action', {
          params: {
            'operation': operation
          }
        }).then((response) => {
          console.log(response.data);
          this.loadJobs()
        })
      }
    },
    data: function () {
      return {
        data: null,
        workunitsHeader: [
          {text: 'Package', align: 'left', value: 'package_id'},
          {text: 'Cracking time', align: 'left', value: 'cracking_time'},
          {text: 'Generated', align: 'right', value: 'time'},
          {text: 'Start index', align: 'right', value: 'start_index'},
          {text: 'Keyspace', align: 'right', value: 'hc_keyspace'},
          {text: 'Retry', align: 'right', value: 'retry'},
          {text: 'Finish', align: 'right', value: 'finished'}
        ],
        jobHeaders: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Attack type', value: 'attack_mode', align: 'right'},
          {text: 'Status', value: 'status', align: 'right'},
          {text: 'Progress', value: 'progress', align: 'right'},
          {text: 'Added', value: 'time', align: 'right'},
          {text: 'Result', value: 'password', align: 'right', sortable: false},
          {text: 'Actions', value: 'name', sortable: false, align: 'right'}
        ],
        jobs_statuses: [
          {
            'text': 'ready',
            'code': 0
          },
          {
            'text': 'finished',
            'code': 1
          },
          {
            'text': 'exhausted',
            'code': 2
          },
          {
            'text': 'malformed',
            'code': 3
          },
          {
            'text': 'timeout',
            'code': 4
          },
          {
            'text': 'running',
            'code': 10
          },
          {
            'text': 'finishing',
            'code': 12
          }
        ],
      }
    }

  }
</script>

<style scoped>
  .dictContentContainer {
    max-width: 600px;
  }

  .width100 {
    width: 100%;
  }

  .max800 {
    max-width: 800px;
  }


</style>
