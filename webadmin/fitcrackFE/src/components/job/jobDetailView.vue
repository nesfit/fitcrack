<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="relative">
    <div v-if="data === null">
      <v-progress-circular indeterminate
                           color="primary"
                           size="65"
                           class="loadingProgress"
                           :width="4">
      </v-progress-circular>
    </div>
    <div v-else>
      <v-breadcrumbs divider="/" class="pb-0">
        <v-breadcrumbs-item>
          <router-link :to="{name: 'jobs'}">Jobs</router-link>
        </v-breadcrumbs-item>
        <v-breadcrumbs-item>
          {{data.name}}
        </v-breadcrumbs-item>
      </v-breadcrumbs>
      <div>
        <v-layout row wrap justify-center>
          <div class="px-2 max100">

            <v-layout row wrap class="mt-3 mb-5 elevation-2 white max500">
              <v-toolbar color="primary" dark card>
                <v-toolbar-title>
                  {{data.name}}
                </v-toolbar-title>
                <v-spacer></v-spacer>
                <v-btn outline small fab color="white" @click.native.stop="showEditJobDialog">
                  <v-icon>edit</v-icon>
                </v-btn>
              </v-toolbar>
              <v-list single-line class="width100">
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Operations:
                  </v-list-tile-action>
                  <v-list-tile-content class="height100">
                    <v-list-tile-title class="text-xs-right height100">
                      <div class="actionsBtns">
                        <v-tooltip top>
                          <v-btn icon class="mx-0" :disabled="data.status !== '0'" slot="activator" @click="operateJob('start')">
                            <v-icon color="success">play_circle_outline</v-icon>
                          </v-btn>
                          <span>Start job</span>
                        </v-tooltip>
                        <v-tooltip top>
                          <v-btn icon class="mx-0" slot="activator" :disabled="data.status >= 10" @click="operateJob('restart')">
                            <v-icon color="info">loop</v-icon>
                          </v-btn>
                          <span>Restart job</span>
                        </v-tooltip>
                        <v-tooltip top>
                          <v-btn icon class="mx-0" :disabled="data.status !== '10'" slot="activator" @click="operateJob('stop')">
                            <v-icon color="error">pause_circle_outline</v-icon>
                          </v-btn>
                          <span>Stop job</span>
                        </v-tooltip>
                      </div>
                    </v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Comment:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">{{data.comment}}</v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Job keyspace:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">{{data.keyspace}}</v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Status:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title v-bind:class="data.status_type + '--text'" class="text-xs-right fw500">
                      <v-tooltip top>
                        <span
                          slot="activator"
                        >
                          {{ data.status_text }}
                        </span>
                        <span>{{ data.status_tooltip }}</span>
                      </v-tooltip>
                    </v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Hash type:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right fw500">{{data.hash_type_name}}</v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1" v-if="data.password !== null">
                  <v-list-tile-action class="pr-3 key">
                    Password:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right fw500">{{data.password}}</v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Added:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">{{ $moment(data.time).format('DD.MM.YYYY HH:mm') }}</v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Cracking time:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">{{data.cracking_time_str}}</v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Progress:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right jobProgress">
                      <v-layout wrap column>
                        <v-flex xs-12 class="height5 text-xs-center">
                          <span
                            class="progressPercentage primary--text">{{ progressToPercentage(data.progress) }}</span>
                        </v-flex>
                        <v-flex xs-12 class="progressLinear">
                          <v-progress-linear
                            height="3"
                            color="primary"
                            :value="data.progress"
                          >
                          </v-progress-linear>
                        </v-flex>
                      </v-layout>
                    </v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Start time:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">
                      <v-list-tile-title class="text-xs-right">{{(data.time_start !== null) ? (
                        $moment(data.time_start).format('DD.MM.YYYY HH:mm')) : 'Not started yet'}}
                      </v-list-tile-title>
                    </v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    End time:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">{{(data.time_end !== null) ? (
                      $moment(data.time_end).format('DD.MM.YYYY HH:mm')) : 'Not set'}}
                    </v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-action class="pr-3 key">
                    Seconds per workunit:
                  </v-list-tile-action>
                  <v-list-tile-content>
                    <v-list-tile-title class="text-xs-right">{{data.seconds_per_job}}
                    </v-list-tile-title>
                  </v-list-tile-content>
                </v-list-tile>

              </v-list>
            </v-layout>


            <combinatorDetail class="max500" :data="data" v-if="data.attack === 'combinator'"/>
            <maskDetail class="max500" :data="data" v-else-if="data.attack === 'mask'"/>
            <dictionaryDetail class="max500" :data="data" v-else-if="data.attack === 'dictionary'"/>
            <combinatorDetail class="max500" :data="data" v-else-if="data.attack === 'hybrid (Wordlist + Mask)'"/>
            <combinatorDetail class="max500" :data="data" v-else-if="data.attack === 'hybrid (Mask + Wordlist)'"/>


            <v-layout row wrap class="mt-3 mb-5 elevation-2 white max500">
              <v-toolbar color="primary" dark card>
                <v-toolbar-title>Hashes</v-toolbar-title>
              </v-toolbar>
              <v-data-table
                :headers="hashHeaders"
                class="width100 maxHeight500"
                :items="data.hashes"
                disable-initial-sort
                :rows-per-page-items='[25,50,100,{"text":"All","value":-1}]'
              >
                <template slot="items" slot-scope="props">
                  <td>{{ props.item.hashText }}</td>
                  <td class="text-xs-right">
                    {{ props.item.password }}
                  </td>
                </template>
              </v-data-table>
            </v-layout>



            <v-layout row wrap class="mt-3 mb-5 elevation-2 white max700">
              <v-toolbar color="primary" dark card>
                <v-toolbar-title>Hosts</v-toolbar-title>
              </v-toolbar>
              <v-list single-line class="width100">
                <v-data-table
                  :headers="hostheaders"
                  :items="data.hosts"
                  disable-initial-sort
                  hide-actions
                >
                  <template slot="items" slot-scope="props">
                    <td>
                      <router-link :to="{ name: 'hostDetail', params: { id: props.item.id}}" class="middle">
                        {{ props.item.domain_name + ' (' + props.item.user.name + ')'}}</router-link>
                    </td>
                    <td class="text-xs-right">{{ props.item.ip_address }}</td>
                    <td class="text-xs-right"  v-bind:class="{
                        'error--text': props.item.last_active.seconds_delta > 61,
                        'success--text': props.item.last_active.seconds_delta < 60 && props.item.last_active.seconds_delta !== null
                      }">
                      <v-icon :title="parseTimeDelta(props.item.last_active.last_seen)" class="inheritColor">fiber_manual_record</v-icon>
                    </td>
                  </template>
                </v-data-table>
                <v-divider></v-divider>
                <v-list-tile class="px-2 py-1">
                  <v-list-tile-content>
                    <v-btn class="ma-0" outline color="primary" @click.native.stop="showMappingHostDialog">Edit
                      mapping
                    </v-btn>
                  </v-list-tile-content>
                </v-list-tile>
              </v-list>
            </v-layout>
          </div>

          <div class="px-3 min500">
            <v-layout row wrap class="mt-3 mb-5 elevation-2 white">
              <v-toolbar color="primary" dark card>
                <v-toolbar-title>Job progress</v-toolbar-title>
              </v-toolbar>
              <fc-graph
                id="progressGraph"
                :data='progressGraph'
                units="%"
                type="job"
              >
              </fc-graph>
            </v-layout>


            <v-layout row wrap class="mt-3 mb-5 elevation-2 white">
              <v-toolbar color="primary" dark card>
                <v-toolbar-title>Hashes in workunit</v-toolbar-title>
              </v-toolbar>
              <fc-graph
                id="hostGraph"
                :data='hostGraph'
                units=" hashes"
                type="host"
              >
              </fc-graph>
            </v-layout>

            <v-layout row wrap class="mt-3 mb-5 elevation-2 white">
              <v-toolbar color="primary" dark card>
                <v-toolbar-title>Host percentage work</v-toolbar-title>
              </v-toolbar>
              <fc-graph
                id="hostPercentageGraph"
                :data='hostPercentageGraph'
              >
              </fc-graph>
            </v-layout>

          </div>
        </v-layout>
        <div class=" mx-3">
          <v-layout row wrap class="mt-3 mb-5 max1000 mx-auto elevation-2 white">
            <v-toolbar color="primary" dark card>
              <v-toolbar-title>Workunits</v-toolbar-title>
            </v-toolbar>
            <v-data-table
              :rows-per-page-items="[15,30,60,{'text':'All','value':-1}]"
              rows-per-page-text="Workunits per page"
              disable-initial-sort
              :headers="workunitsHeader"
              :items="data.workunits"
              class="width100"
            >
              <template slot="items" slot-scope="props">
                <td>
                  <router-link :to="{ name: 'hostDetail', params: { id: props.item.host.id}}" class="middle">
                    {{ props.item.host.domain_name + ' (' + props.item.host.user.name + ')' }}</router-link>
                </td>
                <td class="text-xs-right">
                  <v-progress-circular
                    size="35"
                    :width="1.5"
                    :rotate="270"
                    color="primary"
                    :value="props.item.progress"
                  >
                    <span class="progressPercentageMask">{{ props.item.progress }}</span>
                  </v-progress-circular>
                </td>
                <td class="text-xs-right">{{ props.item.cracking_time_str }}</td>
                <td class="text-xs-right">{{ $moment(props.item.time).format('DD.MM.YYYY HH:mm') }}</td>
                <td class="text-xs-right">{{ props.item.start_index }}</td>
                <td class="text-xs-right">{{ props.item.hc_keyspace }}</td>
                <td class="text-xs-right error--text" v-bind:class="{'success--text': props.item.retry}">{{
                  yesNo(props.item.retry) }}
                </td>
                <td class="text-xs-right error--text" v-bind:class="{'success--text': props.item.finished}">{{
                  yesNo(props.item.finished) }}
                </td>
                <td class="text-xs-right"><v-btn icon small flat color="primary" @click="props.expanded = !props.expanded"><v-icon>insert_drive_file</v-icon></v-btn></td>
              </template>
              <template slot="expand" slot-scope="props">
                <fc-textarea max-height="500" :readonly="true" :value="props.item.result.stderr_out_text"></fc-textarea>
              </template>
            </v-data-table>
          </v-layout>
        </div>

      </div>
    </div>


    <v-dialog v-model="editJobDialog" max-width="490" lazy>
      <v-card class="pt-4" v-if="editJobValues !== null">
        <v-layout row wrap class="px-3">
          <v-flex xs4>
            <v-subheader class="height64">Name:</v-subheader>
          </v-flex>
          <v-flex xs8>
            <v-text-field
              single-line
              v-model="editJobValues.name"
              required
            ></v-text-field>
          </v-flex>
          <v-flex xs4>
            <v-subheader class="height64">Comment:</v-subheader>
          </v-flex>
          <v-flex xs8>
            <v-text-field
              v-model="editJobValues.comment"
            ></v-text-field>
          </v-flex>
          <v-flex xs4>
            <v-subheader class="height64">Start time:</v-subheader>
          </v-flex>
          <v-flex xs5>
            <v-text-field
              :disabled="editJobValues.startNow"
              v-model="editJobValues.time_start"
              flat
              single-line
              label=""
              mask="date-with-time"
            ></v-text-field>
          </v-flex>
          <v-flex xs3>
            <v-checkbox
              label="start now"
              v-model="editJobValues.startNow"
            ></v-checkbox>
          </v-flex>
          <v-flex xs4>
            <v-subheader class="height64">End time:</v-subheader>
          </v-flex>
          <v-flex xs5>
            <v-text-field
              :disabled="editJobValues.endNever"
              v-model="editJobValues.time_end"
              flat
              single-line
              label=""
              mask="date-with-time"
            ></v-text-field>
          </v-flex>
          <v-flex xs3>
            <v-checkbox
              label="End never"
              v-model="editJobValues.endNever"
            ></v-checkbox>
          </v-flex>
          <v-flex xs4>
            <v-subheader class="height64">Seconds per workunit:</v-subheader>
          </v-flex>
          <v-flex xs8>
            <v-text-field
              v-model="editJobValues.seconds_per_job"
            ></v-text-field>
          </v-flex>
        </v-layout>
        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn color="primary" flat @click.native="changeJobSettings">Save</v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>


    <v-dialog v-model="editHostsDialog" max-width="800">
      <v-card>
        <v-data-table
          v-model="newHostsMapping"
          item-key="id"
          select-all
          class="width100"
          :headers="hostHeaders"
          :items="hosts"
          :pagination.sync="paginationHost"
          :total-items="totalHostItems"
          :loading="loadingHosts"
          :rows-per-page-items="[10, 25,50,100]"
          rows-per-page-text="Hosts per page"
          disable-initial-sort
        >
          <template slot="headers" slot-scope="props">
            <tr>
              <th>
                <v-checkbox
                  primary
                  hide-details
                  @click.native="toggleAll"
                  :input-value="props.all"
                  :indeterminate="props.indeterminate"
                ></v-checkbox>
              </th>
              <th
                v-for="header in hostHeaders"
                :key="header.text"
                :class="['column sortable', paginationHost.descending ? 'desc' : 'asc', header.value === paginationHost.sortBy ? 'active' : '']"
                @click="changeSort(header.value)"
              >
                <v-icon small>arrow_upward</v-icon>
                {{ header.text }}
              </th>
            </tr>
          </template>
          <template slot="items" slot-scope="props">
            <tr :active="props.selected" @click="props.selected = !props.selected">
              <td>
                <v-checkbox :input-value="props.selected"></v-checkbox>
              </td>
              <td>
                <router-link :to="{ name: 'hostDetail', params: { id: props.item.id}}" class="middle">{{ props.item.domain_name + ' (' + props.item.user.name + ')'}}</router-link>
              </td>
              <td class="text-xs-right">{{ props.item.ip_address }}</td>
              <td class="text-xs-right"  v-bind:class="{
                  'error--text': props.item.last_active.seconds_delta > 61,
                  'success--text': props.item.last_active.seconds_delta < 60 && props.item.last_active.seconds_delta !== null
                }">
                <v-icon :title="parseTimeDelta(props.item.last_active.last_seen)" class="inheritColor">fiber_manual_record</v-icon>
              </td>
            </tr>
          </template>
        </v-data-table>
        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn color="primary" flat @click.native="changeHostMapping">Save</v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>


  </div>
</template>

<script>
  import combinatorDetail from '@/components/job/attacksDetail/combinator'
  import maskDetail from '@/components/job/attacksDetail/mask'
  import dictionaryDetail from '@/components/job/attacksDetail/dictionary'
  import {DonutChart, LineChart} from 'vue-morris'
  import graph from '@/components/graph/fc_graph'
  import FcTextarea from '@/components/textarea/fc_textarea'

  export default {
    name: "jobDetail",
    components: {
      'fc-graph': graph,
      'combinatorDetail': combinatorDetail,
      'maskDetail': maskDetail,
      'dictionaryDetail': dictionaryDetail,
      'fc-textarea': FcTextarea
    },
    mounted: function () {
      this.loadData();
      this.interval = setInterval(function () {
        this.loadData()
      }.bind(this), 8000)
    },
    beforeDestroy: function () {
      clearInterval(this.interval)
    },

    data: function () {
      return {
        data: null,
        progressGraph: null,
        hostGraph: null,
        hostPercentageGraph: null,
        hostheaders: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'IP address', value: 'ip_adress', align: 'right', sortable: false},
          {text: 'Online', value: 'last_seen', align: 'right', sortable: false}
        ],
        workunitsHeader: [
          {
            text: 'Host',
            value: 'boinc_host_id',
          },
          {text: 'Progress', align: 'right', value: 'progress'},
          {text: 'Cracking time', align: 'right', value: 'cracking_time'},
          {text: 'Generated', align: 'right', value: 'time'},
          {text: 'Start index', align: 'right', value: 'start_index'},
          {text: 'Keyspace', align: 'right', value: 'hc_keyspace'},
          {text: 'Retry', align: 'right', value: 'retry'},
          {text: 'Finished', align: 'right', value: 'finished'},
          {text: 'Log', align: 'left', value: 'test'}
        ],
        hashHeaders: [
          {
            text: 'Hash',
            align: 'left',
            value: 'hash'
          },
          {text: 'Password', value: 'password', align: 'right'}
        ],
        editJobDialog: false,
        editHostsDialog: false,
        totalHostItems: 0,
        paginationHost: {},
        loadingHosts: true,

        hostHeaders: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'IP address', value: 'ip_adress', align: 'right', sortable: false},
          {text: 'Online', value: 'last_seen', align: 'right', sortable: false}
        ],
        hosts: [],
        newHostsMapping: [],
        editJobValues : {
          name: '',
          comment: '',
          seconds_per_job: 3600,
          time_start: this.$moment().format('DD/MM/YYYY HH:mm'),
          time_end:   this.$moment().format('DD/MM/YYYY HH:mm'),
          startNow: false,
          endNever: false
        }
      }
    },
    methods: {
      parseTimeDelta: function (delta) {
        if (delta !== null && delta !== undefined) {
          return this.$moment.utc(delta).fromNow()
        } else {
          return 'Unknown'
        }
      },
      yesNo: function (val) {
        return val ? 'Yes' : 'No'
      },
      loadData: function () {
        this.axios.get(this.$serverAddr + '/jobs/' + this.$route.params.id).then((response) => {
          this.data = response.data
        });

        // if package is finished, we dont need to send this stuffs...
        if (this.data !== null && parseInt(this.data.status) < 5)
          return

        this.axios.get(this.$serverAddr + '/graph/packagesProgress/' + this.$route.params.id).then((response) => {
          this.progressGraph = response.data
        });

        this.axios.get(this.$serverAddr + '/graph/hostsComputing/' + this.$route.params.id).then((response) => {
          this.hostGraph = response.data
        });

        this.axios.get(this.$serverAddr + '/graph/hostPercentage/' + this.$route.params.id).then((response) => {
          this.hostPercentageGraph = response.data
        });
      },
      showEditJobDialog: function() {
        this.editJobValues = {
          name: this.data.name,
          comment: this.data.comment,
          seconds_per_job: this.data.seconds_per_job,
          time_start: this.$moment(this.data.time_start).format('DD/MM/YYYY HH:mm'),
          time_end: this.data.time_end === null ?
            this.$moment().format('DD/MM/YYYY HH:mm') :
            this.$moment(this.data.time_end).format('DD/MM/YYYY HH:mm'),
          startNow: (this.data.time_start === null),
          endNever: (this.data.time_end === null)
        }
        this.editJobDialog = true
      },
      changeJobSettings: function() {
        if (this.editJobValues.startNow) {
          this.editJobValues.time_start = ''
        } else {
          this.editJobValues.time_start = this.$moment(this.editJobValues.time_start, 'DDMMYYYYHHmm').format('DD/MM/YYYY HH:mm')
        }

        if (this.editJobValues.endNever) {
          this.editJobValues.time_end = ''
        } else {
          this.editJobValues.time_end = this.$moment(this.editJobValues.time_end, 'DDMMYYYYHHmm').format('DD/MM/YYYY HH:mm')
        }

        this.axios.put(this.$serverAddr + '/jobs/' + this.data.id , this.editJobValues
        ).then((response) => {
          console.log(response.data);
          this.editJobDialog = false
          this.loadData()
        })
      },
      showMappingHostDialog() {
        this.newHostsMapping = this.data.hosts.slice()
        this.editHostsDialog = true
        this.axios.get(this.$serverAddr + '/hosts', {
          params: {
            'all': true,
            'page': this.paginationHost.page,
            'per_page': this.paginationHost.rowsPerPage,
            'order_by': this.paginationHost.sortBy,
            'descending': this.paginationHost.descending,
            'name': this.search,
            'status': this.status
          }
        }).then((response) => {
          this.hosts = response.data.items
          this.totalItems = response.data.total
          this.loadingHosts = false
        })
      },
      toggleAll() {
        if (this.newHostsMapping.length) this.newHostsMapping = []
        else this.newHostsMapping = this.hosts.slice()
      },
      progressToPercentage: function (progress) {
        return progress.toFixed() + '%'
      },
      changeHostMapping: function () {
        var hostIds = []
        for (let i = 0; i < this.newHostsMapping.length; i++) {
          hostIds.push(this.newHostsMapping[i].id)
        }
        this.axios.post(this.$serverAddr + '/jobs/' + this.data.id + '/host', {
          'newHost_ids': hostIds
        }).then((response) => {
          console.log(response.data);
          this.editHostsDialog = false
          this.loadData()
        })

      },
      operateJob: function (operation) {
        this.axios.get(this.$serverAddr + '/jobs/' + this.data.id + '/action', {
          params: {
            'operation': operation
          }
        }).then((response) => {
          console.log(response.data);
          this.loadData()
        })
      }
    }
  }
</script>

<style scoped>
  .height64 {
    height: 64px;
  }

  .inheritColor {
    color: inherit !important;
  }

  .height100 {
    height: 100%;
  }

  .width100 {
    width: 100%;
  }

  .max700 {
    max-width: 700px;
  }

  .max500 {
    max-width: 500px;
  }


  .key {
    font-weight: bold;
  }

  .progressPercentage {
    text-align: center;
    font-weight: 500;
  }

  .jobProgress {
    height: 35px;
    padding-right: 2px;
  }

  .height5 {
    height: 10px;
  }

  h3 {
    font-weight: normal;
    color: rgba(0, 0, 0, .54);
  }

  .fw500 {
    font-weight: 500;
  }

  .progressPercentageMask {
    font-size: 11px;
  }

  .min500 {
    max-width: 500px;
    width: 100%;
  }

  .graph {
    width: 100%;
  }

  .max100 {
    max-width: 100%;
  }

  .max1000 {
    max-width: 1000px;
  }

  .loadingProgress {
    position: absolute;
    top: 0;
    right: 0;
    left: 0;
    bottom: 0;
    margin: auto;
  }

  .relative {
    position: relative;
    height: 100%;
  }

  .maxHeight500 {
    max-height: 500px;
    overflow: auto;
  }
</style>
