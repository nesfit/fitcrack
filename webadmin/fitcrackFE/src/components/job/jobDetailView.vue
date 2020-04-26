<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="relative">
    <div v-if="data === null">
      <v-progress-circular
        indeterminate
        color="primary"
        size="65"
        class="loadingProgress"
        :width="4"
      />
    </div>
    <div v-else>
      <v-breadcrumbs
        divider="/"
        :items="
          [
            { text: 'Jobs', to: { name: 'jobs' }, exact: true },
            { text: data.name }
          ]"
      />
      <div>
        <v-row 
          justify="center"
          class="mx-3"
        >
          <v-col>
            <v-card 
              class="mb-5 pa-4"
              :style="{backgroundImage: accent}"
            >
              <v-card-title>
                <span class="display-1">
                  {{ data.name }}
                </span>
                <v-spacer />
                <v-btn
                  rounded
                  @click.native.stop="showEditJobDialog"
                >
                  Edit
                  <v-icon 
                    right 
                    small
                  >
                    tune
                  </v-icon>
                </v-btn>
              </v-card-title>
              <v-list
                single-line
              >
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Operations:
                  </v-list-item-action>
                  <v-list-item-content class="height100">
                    <v-list-item-title class="text-right height100">
                      <v-btn
                        v-if="data.hosts.length == 0"
                        class="ma-0"
                        outlined
                        color="warning"
                        @click.native.stop="showMappingHostDialog"
                      >
                        Assign hosts
                      </v-btn>
                      <div
                        v-else
                        class="actionsBtns"
                      >
                        <v-tooltip top>
                          <template v-slot:activator="{ on }">
                            <v-btn
                              icon
                              :disabled="data.status != '0'"
                              v-on="on"
                              @click="operateJob('start')"
                            >
                              <v-icon color="success">
                                mdi-play
                              </v-icon>
                            </v-btn>
                          </template>
                          <span>Start job</span>
                        </v-tooltip>
                        <v-tooltip top>
                          <template v-slot:activator="{ on }">
                            <v-btn
                              icon
                              :disabled="data.status >= 10"
                              v-on="on"
                              @click="operateJob('restart')"
                            >
                              <v-icon color="info">
                                mdi-restart
                              </v-icon>
                            </v-btn>
                          </template>
                          <span>Restart job</span>
                        </v-tooltip>
                        <v-tooltip top>
                          <template v-slot:activator="{ on }">
                            <v-btn
                              icon
                              :disabled="data.status !== '10'"
                              v-on="on"
                              @click="operateJob('stop')"
                            >
                              <v-icon color="error">
                                mdi-pause
                              </v-icon>
                            </v-btn>
                          </template>
                          <span>Stop job</span>
                        </v-tooltip>
                        <v-tooltip top>
                          <template v-slot:activator="{ on }">
                            <v-btn
                              icon
                              v-on="on"
                              @click="operateJob('kill')"
                            >
                              <v-icon color="error">
                                mdi-close
                              </v-icon>
                            </v-btn>
                          </template>
                          <span>Purge job</span>
                        </v-tooltip>
                      </div>
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item 
                  v-if="data.comment"
                  class="px-2 py-1"
                >
                  <v-list-item-action class="pr-3 key">
                    Comment:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right">
                      {{ data.comment }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider v-if="data.comment" />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Job keyspace:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right">
                      {{ data.keyspace }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Status:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title
                      :class="data.status_type + '--text'"
                      class="text-right fw500"
                    >
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <span>
                            {{ data.status_text }}
                          </span>
                        </template>
                        <span>{{ data.status_tooltip }}</span>
                      </v-tooltip>
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Hash type:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right fw500">
                      {{ data.hash_type_name }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-list-item
                  v-if="data.password !== null"
                  class="px-2 py-1"
                >
                  <v-list-item-action class="pr-3 key">
                    Password:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right fw500">
                      {{ data.password }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Added:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right">
                      {{ $moment(data.time).calendar() }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Workunit sum time:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right">
                      {{ data.cracking_time_str }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Job cracking time:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title 
                      v-if="data.time_end"
                      class="text-right"
                    >
                      {{ $moment.utc($moment(data.time_end).diff($moment(data.time_start))).format("HH:mm:ss") }}
                    </v-list-item-title>
                    <v-list-item-title 
                      v-else-if="data.time_start"
                      class="text-right"
                    >
                      {{ $moment.utc($moment().diff($moment(data.time_start))).format("HH:mm:ss") }}
                    </v-list-item-title>
                    <v-list-item-title 
                      v-else
                      class="text-right"
                    >
                      Not started yet
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Progress:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right jobProgress d-flex align-center">
                      <span
                        class="progressPercentage primary--text mr-2"
                      >
                        {{ progressToPercentage(data.progress) }}
                      </span>
                      <v-progress-linear
                        height="3"
                        color="primary"
                        :value="data.progress"
                      />
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Start time:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right">
                      <v-list-item-title class="text-right">
                        {{ (data.time_start !== null) ? (
                          $moment(data.time_start).format('DD.MM.YYYY HH:mm')) : 'Not started yet' }}
                      </v-list-item-title>
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    End time:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right">
                      {{ (data.time_end !== null) ? (
                        $moment(data.time_end).format('DD.MM.YYYY HH:mm')) : 'Not set' }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Efficiency:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right">
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <span v-on="on">{{ efficiency }}</span>
                        </template>
                        <span>Efficiency is computed as sum of cracking time of each workunit divided by count of hosts multipled by total cracking time.</span>
                      </v-tooltip>
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Seconds per workunit:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right">
                      {{ data.seconds_per_job }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
              </v-list>
            </v-card>

            <component
              :is="attackDetailComponent"
              :data="data"
              class="mb-5"
            />

            <v-card class="mb-5">
              <v-card-title>
                Hashes
              </v-card-title>
              <v-data-table
                :headers="hashHeaders"
                class="width100 maxHeight500"
                :items="data.hashes"
                :footer-props="{itemsPerPageOptions: [25,50,100,{text: 'All', value: -1}], itemsPerPageText: 'Hashes per page'}"
              />
            </v-card>



            <v-card class="mb-5">
              <v-card-title>
                Hosts
              </v-card-title>
              <v-list
                single-line
              >
                <v-data-table
                  :headers="hostheaders"
                  :items="data.hosts"
                  hide-default-footer
                  no-data-text="None assigned"
                >
                  <template v-slot:item.name="{ item }">
                    <router-link
                      :to="{ name: 'hostDetail', params: { id: item.id}}"
                      class="middle"
                    >
                      {{ item.domain_name + ' (' + fixUserNameEncoding(item.user.name) + ')' }}
                    </router-link>
                  </template>
                  <template v-slot:item.last_active="{ item }">
                    <span v-if="item.last_active.seconds_delta > 61">{{ parseTimeDelta(item.last_active.last_seen) }}</span>
                    <v-icon
                      v-else
                      color="success"
                    >
                      mdi-power
                    </v-icon>
                  </template>
                </v-data-table>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-content>
                    <v-btn
                      class="ma-0"
                      text
                      color="primary"
                      @click.native.stop="showMappingHostDialog"
                    >
                      Edit
                      mapping
                    </v-btn>
                  </v-list-item-content>
                </v-list-item>
              </v-list>
            </v-card>
          </v-col>

          <v-col>
            <v-card class="mb-5">
              <v-card-title>
                Job progress
              </v-card-title>
              <fc-graph
                id="progressGraph"
                :data="progressGraph"
                units="%"
                type="job"
              />
            </v-card>


            <v-card class="mb-5">
              <v-card-title>
                Hashes in workunit
              </v-card-title>
              <fc-graph
                id="hostGraph"
                :data="hostGraph"
                units=" hashes"
                type="host"
              />
            </v-card>

            <v-card class="mb-5">
              <v-card-title>
                Host percentage work
              </v-card-title>
              <fc-graph
                id="hostPercentageGraph"
                class="py-8"
                :data="hostPercentageGraph"
              />
            </v-card>

            <v-card class="mt-3 mb-5 elevation-2">
              <v-card-title>
                Status history
              </v-card-title>
              <v-list
                single-line
              >
                <v-data-table
                  :headers="statusHeaders"
                  :items="statusHistory"
                  :footer-props="{itemsPerPageOptions: [5,25,50,{text: 'All', value: -1}]}"
                >
                  <template v-slot:item.time="{ item }">
                    {{ $moment(item.time).format('DD.MM.YYYY HH:mm') }}
                  </template>
                  <template v-slot:item.status="{ item }">
                    <v-tooltip top>
                      <template v-slot:activator="{ on }">
                        <span 
                          :class="item.status_type + '--text'"
                          v-on="on"
                        >
                          {{ item.status_text }}
                        </span>
                      </template>
                      <span>{{ item.status_tooltip }}</span>
                    </v-tooltip>
                  </template>
                </v-data-table>
              </v-list>
            </v-card>
          </v-col>
        </v-row>

        <div class="mx-3 mb-5">
          <v-card>
            <v-card-title>
              {{ 'Workunits | Work: ' + workunitTitle.valid + ' | Benchmark: ' + workunitTitle.benchmarks + ' | Avg keyspace: ' + workunitTitle.avgKeyspace.toLocaleString() }}
            </v-card-title>

            <div class="workunit-parent">
              <div
                v-for="workunit in workunitsGraphical"
                :key="workunit.id"
                :style="{ 'flex-grow': workunit.keyspace, 'background-color': workunit.color }"
                class="workunit-child"
              >
                <v-tooltip bottom>
                  <template v-slot:activator="{ on }">
                    <div v-on="on">
&nbsp;
                    </div>
                  </template>
                  <span>{{ workunit.text }}</span>
                </v-tooltip>
              </div>
            </div>

            <v-data-table
              :footer-props="{itemsPerPageOptions: [15,30,60,{text: 'All', value: -1}], itemsPerPageText: 'Workunits per page'}"
              :headers="workunitsHeader"
              :items="data.workunits"
              show-expand
            >
              <template v-slot:item.boinc_host_id="{ item }">
                <router-link
                  :to="{ name: 'hostDetail', params: { id: item.host.id}}"
                  class="middle"
                >
                  {{ item.host.domain_name + ' (' + fixUserNameEncoding(item.host.user.name) + ')' }}
                </router-link>
              </template>
              <template v-slot:item.progress="{ item }">
                <div class="d-flex align-center justify-end">
                  <span class="mr-2">{{ progressToPercentage(item.progress) }}</span>
                  <v-progress-circular
                    size="18"
                    :width="3"
                    :rotate="270"
                    color="primary"
                    class="jobProgress"
                    :value="item.progress"
                  />
                </div>
              </template>
              <template v-slot:item.time="{ item }">
                {{ $moment(item.time).format('DD.MM.YYYY HH:mm') }}
              </template>
              <template v-slot:item.retry="{ item }">
                <v-chip
                  small
                  :color="item.retry ? 'error' : 'success'"
                >
                  {{ yesNo(item.retry) }}
                </v-chip>
              </template>
              <template v-slot:item.finished="{ item }">
                <v-chip
                  small
                  :color="item.finished ? 'success' : 'error'"
                >
                  {{ yesNo(item.finished) }}
                </v-chip>
              </template>
              <template
                v-slot:expanded-item="{ headers, item }"
              >
                <td :colspan="headers.length">
                  <fc-textarea
                    max-height="500"
                    :readonly="true"
                    :value="item.result.stderr_out_text"
                  />
                </td>
              </template>
            </v-data-table>
          </v-card>
        </div>
      </div>
    </div>


    <v-dialog
      v-model="editJobDialog"
      max-width="700"
    >
      <v-card
        v-if="editJobValues !== null"
        class="pt-4"
      >
        <v-card-title>
          Edit job
        </v-card-title>
        <v-card-text>
          <v-row>
            <v-col cols="4">
              <v-subheader class="height64">
                Name:
              </v-subheader>
            </v-col>
            <v-col cols="8">
              <v-text-field
                v-model="editJobValues.name"
                single-line
                required
              />
            </v-col>
            <v-col cols="4">
              <v-subheader class="height64">
                Comment:
              </v-subheader>
            </v-col>
            <v-col cols="8">
              <v-text-field
                v-model="editJobValues.comment"
              />
            </v-col>
            <v-col cols="4">
              <v-subheader class="height64">
                Start time:
              </v-subheader>
            </v-col>
            <v-col cols="6">
              <dt-picker
                v-model="editJobValues.time_start"
                :disabled="editJobValues.startNow"
                text
                single-line
              />
            </v-col>
            <v-col cols="2">
              <v-checkbox
                v-model="editJobValues.startNow"
                label="None"
              />
            </v-col>
            <v-col cols="4">
              <v-subheader class="height64">
                End time:
              </v-subheader>
            </v-col>
            <v-col cols="6">
              <dt-picker
                v-model="editJobValues.time_end"
                :disabled="editJobValues.endNever"
                text
                single-line
              />
            </v-col>
            <v-col cols="2">
              <v-checkbox
                v-model="editJobValues.endNever"
                label="Never"
              />
            </v-col>
            <v-col cols="4">
              <v-subheader class="height64">
                Seconds per workunit:
              </v-subheader>
            </v-col>
            <v-col cols="8">
              <v-text-field
                v-model="editJobValues.seconds_per_job"
                min="10"
              />
            </v-col>

              <v-col v-if="editJobValues.attack_mode == 8" cols="12">
                <v-checkbox
                  v-model="editJobValues.check_duplicates"
                  label="Check password duplicates"
                />
                <v-checkbox
                  v-model="editJobValues.case_permute"
                  label="Case permutation"
                />
                <v-checkbox
                  v-model="editJobValues.shuffle_dict"
                  label="Purple Rain Attack"
                />
              </v-col>

              <v-col v-if="editJobValues.attack_mode == 8" cols="8">
                <v-subheader class="height64">
                  Minimal length of passwords (1 - 32):
                </v-subheader>
              </v-col>
              <v-col v-if="editJobValues.attack_mode == 8" cols="4">
                <v-text-field
                  v-model="editJobValues.min_password_len"
                  text
                  single-line
                  required
                  type="number"
                  min="1"
                  max="32"
                />
              </v-col>
              <v-col v-if="editJobValues.attack_mode == 8" cols="8">
                <v-subheader class="height64">
                  Maximal length of passwords (1 - 32):
                </v-subheader>
              </v-col>
              <v-col v-if="editJobValues.attack_mode == 8" cols="4">
                <v-text-field
                  v-model="editJobValues.max_password_len"
                  text
                  single-line
                  required
                  type="number"
                  min="1"
                  max="32"
                />
              </v-col>
              <v-col v-if="editJobValues.attack_mode == 8" cols="8">
                <v-subheader class="height64">
                  Minimal number of elements per chain (1 - 16):
                </v-subheader>
              </v-col>
              <v-col v-if="editJobValues.attack_mode == 8" cols="4">
                <v-text-field
                  v-model="editJobValues.min_elem_in_chain"
                  text
                  single-line
                  required
                  type="number"
                  min="1"
                  max="16"
                />
              </v-col>
              <v-col v-if="editJobValues.attack_mode == 8" cols="8">
                <v-subheader class="height64">
                  Maximal number of elements per chain (1 - 16):
                </v-subheader>
              </v-col>
              <v-col v-if="editJobValues.attack_mode == 8" cols="4">
                <v-text-field
                  v-model="editJobValues.max_elem_in_chain"
                  text
                  single-line
                  required
                  type="number"
                  min="1"
                  max="16"
                />
              </v-col>
              <v-col v-if="editJobValues.attack_mode == 8" cols="8">
                <v-subheader class="height64">
                  Generate random rules:
                </v-subheader>
              </v-col>
              <v-col v-if="editJobValues.attack_mode == 8" cols="4">
                <v-text-field
                  v-model="editJobValues.generate_random_rules"
                  text
                  single-line
                  required
                  type="number"
                  min="0"
                />
              </v-col>

          </v-row>
        </v-card-text>
        <v-card-actions>
          <v-spacer />
          <v-btn
            color="primary"
            text
            @click.stop="editJobDialog=false"
          >
            Cancel
          </v-btn>
          <v-btn
            color="primary"
            text
            @click.native="changeJobSettings"
          >
            Save
          </v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>


    <v-dialog
      v-model="editHostsDialog"
      max-width="800"
    >
      <v-card>
        <v-card-title>
          Host mapping
        </v-card-title>
        <v-card-text>
          <host-selector 
            v-model="newHostsMapping" 
            select-all
            :auto-refresh="editHostsDialog"
          />
        </v-card-text>
        <v-card-actions>
          <v-spacer />
          <v-btn
            text
            @click="editHostsDialog = false"
          >
            Cancel
          </v-btn>
          <v-btn
            color="primary"
            text
            @click.native="changeHostMapping"
          >
            Save
          </v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>
  </div>
</template>

<script>
  import iconv from 'iconv-lite'
  import combinatorDetail from '@/components/job/attacksDetail/combinator'
  import maskDetail from '@/components/job/attacksDetail/mask'
  import dictionaryDetail from '@/components/job/attacksDetail/dictionary'
  import pcfgDetail from '@/components/job/attacksDetail/pcfg'
  import princeDetail from '@/components/job/attacksDetail/prince'
  import {DonutChart, LineChart} from 'vue-morris'
  import graph from '@/components/graph/fc_graph'
  import FcTextarea from '@/components/textarea/fc_textarea'
  import hostSelector from '@/components/selector/hostSelector'
  import dtPicker from '@/components/picker/datetime'

  export default {
    name: "JobDetail",
    components: {
      'fc-graph': graph,
      'combinatorDetail': combinatorDetail,
      'maskDetail': maskDetail,
      'dictionaryDetail': dictionaryDetail,
      'pcfgDetail': pcfgDetail,
      'princeDetail': princeDetail,
      'fc-textarea': FcTextarea,
      hostSelector,
      dtPicker
    },

    data: function () {
      return {
        data: null,
        efficiency: 0,
        progressGraph: null,
        hostGraph: null,
        hostPercentageGraph: null,
        statusHeaders: [
          {text: 'Time', value: 'time', align: 'start'},
          {text: 'Status', value: 'status', align: 'end'},
        ],
        statusHistory: [],
        hostheaders: [
          {
            text: 'Name',
            align: 'start',
            value: 'name'
          },
          {text: 'IP address', value: 'ip_address', align: 'end', sortable: false},
          {text: 'Online', value: 'last_active', align: 'end', sortable: false}
        ],
        workunitTitle: {
          valid: 0,
          benchmarks: 0,
          avgKeyspace: 0,
          efectivity: 0,

        },
        workunitsHeader: [
          {
            text: 'Host',
            value: 'boinc_host_id',
          },
          {text: 'Progress', align: 'end', value: 'progress'},
          {text: 'Cracking time', align: 'end', value: 'cracking_time_str'},
          {text: 'Generated', align: 'end', value: 'time'},
          {text: 'Start index', align: 'end', value: 'start_index'},
          {text: 'Keyspace', align: 'end', value: 'hc_keyspace'},
          {text: 'Retry', align: 'center', value: 'retry'},
          {text: 'Finished', align: 'center', value: 'finished'},
          {text: 'Log', align: 'center', value: 'data-table-expand'}
        ],
        workunitsGraphical: [],
        hashHeaders: [
          {
            text: 'Hash',
            align: 'start',
            value: 'hashText'
          },
          {text: 'Password', value: 'password', align: 'end'}
        ],
        editJobDialog: false,
        editHostsDialog: false,
        newHostsMapping: [],
        editJobValues : {
          name: '',
          comment: '',
          seconds_per_job: 3600,
          time_start: this.$moment().toISOString(true).slice(0, 16),
          time_end:   this.$moment().toISOString(true).slice(0, 16),
          startNow: false,
          endNever: false,
          check_duplicates: false,
          case_permute: false,
          shuffle_dict: false,
          min_password_len: 1,
          max_password_len: 8,
          min_elem_in_chain: 1,
          max_elem_in_chain: 8,
          generate_random_rules: 0,
        }
      }
    },
    computed: {
      attackDetailComponent () {
        switch (this.data.attack) {
          case 'mask':
            return 'maskDetail'
          case 'dictionary':
            return 'dictionaryDetail'
          case 'pcfg':
            return 'pcfgDetail'
          case 'prince':
            return 'princeDetail'
          default:
            return 'combinatorDetail'
        }
      },
      accent () {
        let hue = -1
        switch (this.data.status_text) {
          case 'finished': hue = 110; break
          case 'ready': hue = 210; break
          case 'exhausted': hue = 350; break
        }
        if (this.data.hosts.length == 0) hue = 40
        if (hue == -1) return 'none'
        return `linear-gradient(to bottom, hsla(${hue}, 90%, 50%, 40%), transparent 20%)`
      }
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
        this.axios.get(this.$serverAddr + '/job/' + this.$route.params.id).then((response) => {
          this.data = response.data;

          // Computing of counts and avg keyspace
          if (this.data.workunits.length > 0) {
            const validWorkunits = this.data.workunits.filter(workunit =>workunit.hc_keyspace !== 0);
            this.workunitTitle.valid = validWorkunits.length;
            this.workunitTitle.benchmarks = this.data.workunits.length - validWorkunits.length;
            if (validWorkunits.length > 0)
              this.workunitTitle.avgKeyspace = (validWorkunits.map(workunit => workunit.hc_keyspace).reduce((a, b) => a + b) / validWorkunits.length);

            // Computing of efficiency
            if(this.data.hosts.length === 0){
              this.efficiency = "No hosts";
            }

            // If Job is finished
            else if (validWorkunits.length > 0) {
              if (this.data.cracking_time == 0) {
                this.efficiency = "-";
              } else {
                const efficiency = (validWorkunits.map(workunit => workunit.cracking_time).reduce((a, b) => a + b)) / (this.data.hosts.length * this.data.cracking_time);

                this.efficiency = (efficiency * 100).toFixed(2) + ' %';
              }
            } else {
              this.efficiency = "No workunits yet";
            }

            // Prepare objects for workunits
            this.workunitsGraphical = validWorkunits.map(workunit => {
              //console.log(workunit);

              // retry && finished
              let color = "";

              if (!workunit.retry && workunit.finished) {
                color = 'lightgreen';
              } else if (!workunit.retry && !workunit.finished){
                color = 'yellow';
              } else if (workunit.retry && !workunit.finished) {
                color = 'orange';
              } else {
                color = 'green';
              }

              return {
                id: workunit.id,
                keyspace: workunit.hc_keyspace,
                color: color,
                text: "Id: " + workunit.id + " | Keyspace: " + workunit.hc_keyspace + " | Retry: " + workunit.retry + " | Finished: " + workunit.finished
              }
            });
          }
        });

        this.axios.get(this.$serverAddr + '/status/' + this.$route.params.id)
                .then((result) => {
                  this.statusHistory = result.data.items;
                })
                .catch((reason => {
                  console.log("An error ocurred while fetching status", reason);
                }));

        // if job is finished, we dont need to send this stuff...
        if (this.data !== null && parseInt(this.data.status) < 5)
          return

        this.axios.get(this.$serverAddr + '/graph/jobsProgress/' + this.$route.params.id).then((response) => {
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
          attack_mode: this.data.attack_mode,
          name: this.data.name,
          comment: this.data.comment,
          seconds_per_job: this.data.seconds_per_job,
          time_start: this.$moment(this.data.time_start).toISOString(true).slice(0, 16),
          time_end: this.data.time_end === null ?
            this.$moment().toISOString(true).slice(0, 16) :
            this.$moment(this.data.time_end).toISOString(true).slice(0, 16),
          startNow: (this.data.time_start === null),
          endNever: (this.data.time_end === null),
          check_duplicates: this.data.check_duplicates,
          case_permute: this.data.case_permute,
          shuffle_dict: this.data.shuffle_dict,
          min_password_len: this.data.min_password_len,
          max_password_len: this.data.max_password_len,
          min_elem_in_chain: this.data.min_elem_in_chain,
          max_elem_in_chain: this.data.max_elem_in_chain,
          generate_random_rules: this.data.generate_random_rules,
        }
        this.editJobDialog = true
      },
      changeJobSettings: function() {
        if (this.editJobValues.startNow) {
          this.editJobValues.time_start = ''
        }

        if (this.editJobValues.endNever) {
          this.editJobValues.time_end = ''
        }

        this.axios.put(this.$serverAddr + '/job/' + this.data.id , this.editJobValues
        ).then((response) => {
          console.log(response.data);
          this.editJobDialog = false
          this.loadData()
        })
      },
      showMappingHostDialog() {
        this.newHostsMapping = this.data.hosts.slice()
        this.editHostsDialog = true
      },
      progressToPercentage: function (progress) {
        if(progress > 100){
          progress = 100
        }
        return progress.toFixed() + '%'
      },
      changeHostMapping: function () {
        var hostIds = []
        for (let i = 0; i < this.newHostsMapping.length; i++) {
          hostIds.push(this.newHostsMapping[i].id)
        }
        this.axios.post(this.$serverAddr + '/job/' + this.data.id + '/host', {
          'newHost_ids': hostIds
        }).then((response) => {
          console.log(response.data);
          this.editHostsDialog = false
          this.loadData()
        })

      },
      operateJob: function (operation) {
        this.axios.get(this.$serverAddr + '/job/' + this.data.id + '/action', {
          params: {
            'operation': operation
          }
        }).then((response) => {
          console.log(response.data);
          this.loadData()
          if (operation === "kill") {
            this.axios.get(this.$serverAddr + '/graph/jobsProgress/' + this.$route.params.id).then((response) => {
              this.progressGraph = response.data
            });

            this.axios.get(this.$serverAddr + '/graph/hostsComputing/' + this.$route.params.id).then((response) => {
              this.hostGraph = response.data
            });

            this.axios.get(this.$serverAddr + '/graph/hostPercentage/' + this.$route.params.id).then((response) => {
              this.hostPercentageGraph = response.data
            });
          }
        })
      },
      fixUserNameEncoding : function(username) {
          /* Boinc DB uses latin1_swedish encoding, which breaks names with special characters,
          which are not supported in this encoding. Fix it by converting name to utf8. */
          return iconv.decode(iconv.encode(username, 'latin1'), 'utf-8')
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

  .workunit-parent {

    overflow: auto;
    height: 55px;
    display: flex;
    padding-left: 20px;
    padding-right: 20px;
    padding-top: 10px;
    width: 100%;
  }

  .workunit-child {
    height: 20px;
    margin: 0 2px;
    border: 1px grey solid;
    border-radius: 2px;
    -webkit-box-sizing: border-box;
    -moz-box-sizing: border-box;
    box-sizing: border-box;
  }

</style>
