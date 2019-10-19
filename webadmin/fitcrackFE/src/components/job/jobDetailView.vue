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
        class="pb-0"
      >
        <v-breadcrumbs-item>
          <router-link :to="{name: 'jobs'}">
            Jobs
          </router-link>
        </v-breadcrumbs-item>
        <v-breadcrumbs-item>
          {{ data.name }}
        </v-breadcrumbs-item>
      </v-breadcrumbs>
      <div>
        <v-row justify="center">
          <div class="px-2 max100">
            <v-row class="mt-3 mb-5 elevation-2 max500">
              <v-toolbar
                color="primary"
                dark
                flat
              >
                <v-toolbar-title>
                  {{ data.name }}
                </v-toolbar-title>
                <v-spacer />
                <v-btn
                  outlined
                  small
                  fab
                  color="white"
                  @click.native.stop="showEditJobDialog"
                >
                  <v-icon>edit</v-icon>
                </v-btn>
              </v-toolbar>
              <v-list
                single-line
                class="width100"
              >
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Operations:
                  </v-list-item-action>
                  <v-list-item-content class="height100">
                    <v-list-item-title class="text-right height100">
                      <div class="actionsBtns">
                        <v-tooltip top>
                          <template v-slot:activator="{ on }">
                            <v-btn
                              icon
                              class="mx-0"
                              :disabled="data.status !== '0'"
                              v-on="on"
                              @click="operateJob('start')"
                            >
                              <v-icon color="success">
                                play_circle_outlined
                              </v-icon>
                            </v-btn>
                          </template>
                          <span>Start job</span>
                        </v-tooltip>
                        <v-tooltip top>
                          <template v-slot:activator="{ on }">
                            <v-btn
                              icon
                              class="mx-0"
                              :disabled="data.status >= 10"
                              v-on="on"
                              @click="operateJob('restart')"
                            >
                              <v-icon color="info">
                                loop
                              </v-icon>
                            </v-btn>
                          </template>
                          <span>Restart job</span>
                        </v-tooltip>
                        <v-tooltip top>
                          <template v-slot:activator="{ on }">
                            <v-btn
                              icon
                              class="mx-0"
                              :disabled="data.status !== '10'"
                              v-on="on"
                              @click="operateJob('stop')"
                            >
                              <v-icon color="error">
                                pause_circle_outlined
                              </v-icon>
                            </v-btn>
                          </template>
                          <span>Stop job</span>
                        </v-tooltip>
                        <v-tooltip top>
                          <template v-slot:activator="{ on }">
                            <v-btn
                              icon
                              class="mx-0"
                              v-on="on"
                              @click="operateJob('kill')"
                            >
                              <v-icon color="error">
                                cancel
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
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Comment:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right">
                      {{ data.comment }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
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
                        <span
                          slot="activator"
                        >
                          {{ data.status_text }}
                        </span>
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
                <v-divider />
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
                    <v-list-item-title class="text-right">
                      {{ $moment.utc($moment(data.time_end).diff($moment(data.time_start))).format("HH:mm:ss") }}
                    </v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-action class="pr-3 key">
                    Progress:
                  </v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title class="text-right jobProgress">
                      <v-row column>
                        <v-col class="height5 text-center xs-12">
                          <span
                            class="progressPercentage primary--text"
                          >{{ progressToPercentage(data.progress) }}</span>
                        </v-col>
                        <v-col class="progressLinear xs-12">
                          <v-progress-linear
                            height="3"
                            color="primary"
                            :value="data.progress"
                          />
                        </v-col>
                      </v-row>
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
            </v-row>


            <combinatorDetail
              v-if="data.attack === 'combinator'"
              class="max500"
              :data="data"
            />
            <maskDetail
              v-else-if="data.attack === 'mask'"
              class="max500"
              :data="data"
            />
            <dictionaryDetail
              v-else-if="data.attack === 'dictionary'"
              class="max500"
              :data="data"
            />
            <combinatorDetail
              v-else-if="data.attack === 'hybrid (Wordlist + Mask)'"
              class="max500"
              :data="data"
            />
            <combinatorDetail
              v-else-if="data.attack === 'hybrid (Mask + Wordlist)'"
              class="max500"
              :data="data"
            />
            <pcfgDetail
              v-else-if="data.attack === 'PCFG'"
              class="max500"
              :data="data"
            />


            <v-row class="mt-3 mb-5 elevation-2 max500">
              <v-toolbar
                color="primary"
                dark
                flat
              >
                <v-toolbar-title>Hashes</v-toolbar-title>
              </v-toolbar>
              <v-data-table
                :headers="hashHeaders"
                class="width100 maxHeight500"
                :items="data.hashes"
                
                :rows-per-page-items="[25,50,100,{&quot;text&quot;:&quot;All&quot;,&quot;value&quot;:-1}]"
              >
                <template
                  slot="items"
                  slot-scope="props"
                >
                  <td>{{ props.item.hashText }}</td>
                  <td class="text-right">
                    {{ props.item.password }}
                  </td>
                </template>
              </v-data-table>
            </v-row>



            <v-row class="mt-3 mb-5 elevation-2 max700">
              <v-toolbar
                color="primary"
                dark
                flat
              >
                <v-toolbar-title>Hosts</v-toolbar-title>
              </v-toolbar>
              <v-list
                single-line
                class="width100"
              >
                <v-data-table
                  :headers="hostheaders"
                  :items="data.hosts"
                  
                  hide-default-footer
                >
                  <template
                    slot="items"
                    slot-scope="props"
                  >
                    <td>
                      <router-link
                        :to="{ name: 'hostDetail', params: { id: props.item.id}}"
                        class="middle"
                      >
                        {{ props.item.domain_name + ' (' + props.item.user.name + ')' }}
                      </router-link>
                    </td>
                    <td class="text-right">
                      {{ props.item.ip_address }}
                    </td>
                    <td
                      class="text-right"
                      :class="{
                        'error--text': props.item.last_active.seconds_delta > 61,
                        'success--text': props.item.last_active.seconds_delta < 60 && props.item.last_active.seconds_delta !== null
                      }"
                    >
                      <v-icon
                        :title="parseTimeDelta(props.item.last_active.last_seen)"
                        class="inheritColor"
                      >
                        fiber_manual_record
                      </v-icon>
                    </td>
                  </template>
                </v-data-table>
                <v-divider />
                <v-list-item class="px-2 py-1">
                  <v-list-item-content>
                    <v-btn
                      class="ma-0"
                      outlined
                      color="primary"
                      @click.native.stop="showMappingHostDialog"
                    >
                      Edit
                      mapping
                    </v-btn>
                  </v-list-item-content>
                </v-list-item>
              </v-list>
            </v-row>
          </div>

          <div class="px-3 min500">
            <v-row class="mt-3 mb-5 elevation-2">
              <v-toolbar
                color="primary"
                dark
                flat
              >
                <v-toolbar-title>Job progress</v-toolbar-title>
              </v-toolbar>
              <fc-graph
                id="progressGraph"
                :data="progressGraph"
                units="%"
                type="job"
              />
            </v-row>


            <v-row class="mt-3 mb-5 elevation-2">
              <v-toolbar
                color="primary"
                dark
                flat
              >
                <v-toolbar-title>Hashes in workunit</v-toolbar-title>
              </v-toolbar>
              <fc-graph
                id="hostGraph"
                :data="hostGraph"
                units=" hashes"
                type="host"
              />
            </v-row>

            <v-row class="mt-3 mb-5 elevation-2">
              <v-toolbar
                color="primary"
                dark
                flat
              >
                <v-toolbar-title>Host percentage work</v-toolbar-title>
              </v-toolbar>
              <fc-graph
                id="hostPercentageGraph"
                :data="hostPercentageGraph"
              />
            </v-row>

            <v-row class="mt-3 mb-5 elevation-2">
              <v-toolbar
                color="primary"
                dark
                flat
              >
                <v-toolbar-title>Status history</v-toolbar-title>
              </v-toolbar>
              <v-list
                single-line
                class="width100"
              >
                <v-data-table
                  :headers="statusHeaders"
                  :items="statusHistory"
                  :rows-per-page-items="[5,10,15,{&quot;text&quot;:&quot;All&quot;,&quot;value&quot;:-1}]"
                >
                  <template
                    slot="items"
                    slot-scope="props"
                  >
                    <td class="text-left">
                      {{ $moment(props.item.time).format('DD.MM.YYYY HH:mm')
                      }}
                    </td>
                    <td
                      class="text-right text-right fw500"
                      :class="props.item.status_type + '--text'"
                    >
                      <v-tooltip top>
                        <template v-slot:activator="{ on }">
                          <span v-on="on">
                            {{ props.item.status_text }}
                          </span>
                        </template>
                        <span>{{ props.item.status_tooltip }}</span>
                      </v-tooltip>
                    </td>
                  </template>
                </v-data-table>
              </v-list>
            </v-row>
          </div>
        </v-row>
        <div class=" mx-3">
          <v-row class="mt-3 mb-5 max1000 mx-auto elevation-2">
            <v-toolbar
              color="primary"
              dark
              flat
            >
              <v-toolbar-title
                v-text="'Workunits | Work: ' + workunitTitle.valid + ' | Benchmark: ' + workunitTitle.benchmarks + ' | Avg keyspace: ' + workunitTitle.avgKeyspace.toLocaleString()"
              /></v-toolbar-title>
            </v-toolbar>

            <div class="workunit-parent">
              <div
                v-for="workunit in workunitsGraphical"
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
              :rows-per-page-items="[15,30,60,{'text':'All','value':-1}]"
              rows-per-page-text="Workunits per page"
              
              :headers="workunitsHeader"
              :items="data.workunits"
              class="width100"
            >
              <template
                slot="items"
                slot-scope="props"
              >
                <td>
                  <router-link
                    :to="{ name: 'hostDetail', params: { id: props.item.host.id}}"
                    class="middle"
                  >
                    {{ props.item.host.domain_name + ' (' + props.item.host.user.name + ')' }}
                  </router-link>
                </td>
                <td class="text-right">
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
                <td class="text-right">
                  {{ props.item.cracking_time_str }}
                </td>
                <td class="text-right">
                  {{ $moment(props.item.time).format('DD.MM.YYYY HH:mm') }}
                </td>
                <td class="text-right">
                  {{ props.item.start_index }}
                </td>
                <td class="text-right">
                  {{ props.item.hc_keyspace }}
                </td>
                <td
                  class="text-right error--text"
                  :class="{'success--text': props.item.retry}"
                >
                  {{
                    yesNo(props.item.retry) }}
                </td>
                <td
                  class="text-right error--text"
                  :class="{'success--text': props.item.finished}"
                >
                  {{
                    yesNo(props.item.finished) }}
                </td>
                <td class="text-right">
                  <v-btn
                    icon
                    small
                    text
                    color="primary"
                    @click="props.expanded = !props.expanded"
                  >
                    <v-icon>insert_drive_file</v-icon>
                  </v-btn>
                </td>
              </template>
              <template
                slot="expand"
                slot-scope="props"
              >
                <fc-textarea
                  max-height="500"
                  :readonly="true"
                  :value="props.item.result.stderr_out_text"
                />
              </template>
            </v-data-table>
          </v-row>
        </div>
      </div>
    </div>


    <v-dialog
      v-model="editJobDialog"
      max-width="490"
      lazy
    >
      <v-card
        v-if="editJobValues !== null"
        class="pt-4"
      >
        <v-row class="px-3">
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
          <v-col cols="5">
            <v-text-field
              v-model="editJobValues.time_start"
              :disabled="editJobValues.startNow"
              text
              single-line
              label=""
              mask="date-with-time"
            />
          </v-col>
          <v-col cols="3">
            <v-checkbox
              v-model="editJobValues.startNow"
              label="start now"
            />
          </v-col>
          <v-col cols="4">
            <v-subheader class="height64">
              End time:
            </v-subheader>
          </v-col>
          <v-col cols="5">
            <v-text-field
              v-model="editJobValues.time_end"
              :disabled="editJobValues.endNever"
              text
              single-line
              label=""
              mask="date-with-time"
            />
          </v-col>
          <v-col cols="3">
            <v-checkbox
              v-model="editJobValues.endNever"
              label="End never"
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
            />
          </v-col>
        </v-row>
        <v-card-actions>
          <v-spacer />
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
        <v-data-table
          v-model="newHostsMapping"
          item-key="id"
          select-all
          class="width100"
          :headers="hostHeaders"
          :items="hosts"
          :pagination.sync="paginationHost"
          :server-items-length="totalHostItems"
          :loading="loadingHosts"
          :rows-per-page-items="[10, 25,50,100]"
          rows-per-page-text="Hosts per page"
          
        >
          <template
            slot="headers"
            slot-scope="props"
          >
            <tr>
              <th>
                <v-checkbox
                  primary
                  hide-details
                  :input-value="props.all"
                  :indeterminate="props.indeterminate"
                  @click.native="toggleAll"
                />
              </th>
              <th
                v-for="header in hostHeaders"
                :key="header.text"
                :class="['column sortable', paginationHost.descending ? 'desc' : 'asc', header.value === paginationHost.sortBy ? 'active' : '']"
                @click="changeSort(header.value)"
              >
                <v-icon small>
                  arrow_upward
                </v-icon>
                {{ header.text }}
              </th>
            </tr>
          </template>
          <template
            slot="items"
            slot-scope="props"
          >
            <tr
              :active="props.selected"
              @click="props.selected = !props.selected"
            >
              <td>
                <v-checkbox :input-value="props.selected" />
              </td>
              <td>
                <router-link
                  :to="{ name: 'hostDetail', params: { id: props.item.id}}"
                  class="middle"
                >
                  {{ props.item.domain_name + ' (' + props.item.user.name + ')' }}
                </router-link>
              </td>
              <td class="text-right">
                {{ props.item.ip_address }}
              </td>
              <td
                class="text-right"
                :class="{
                  'error--text': props.item.last_active.seconds_delta > 61,
                  'success--text': props.item.last_active.seconds_delta < 60 && props.item.last_active.seconds_delta !== null
                }"
              >
                <v-icon
                  :title="parseTimeDelta(props.item.last_active.last_seen)"
                  class="inheritColor"
                >
                  fiber_manual_record
                </v-icon>
              </td>
            </tr>
          </template>
        </v-data-table>
        <v-card-actions>
          <v-spacer />
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
  import combinatorDetail from '@/components/job/attacksDetail/combinator'
  import maskDetail from '@/components/job/attacksDetail/mask'
  import dictionaryDetail from '@/components/job/attacksDetail/dictionary'
  import pcfgDetail from '@/components/job/attacksDetail/pcfg'
  import {DonutChart, LineChart} from 'vue-morris'
  import graph from '@/components/graph/fc_graph'
  import FcTextarea from '@/components/textarea/fc_textarea'

  export default {
    name: "JobDetail",
    components: {
      'fc-graph': graph,
      'combinatorDetail': combinatorDetail,
      'maskDetail': maskDetail,
      'dictionaryDetail': dictionaryDetail,
      'pcfgDetail': pcfgDetail,
      'fc-textarea': FcTextarea
    },

    data: function () {
      return {
        data: null,
        efficiency: 0,
        progressGraph: null,
        hostGraph: null,
        hostPercentageGraph: null,
        statusHeaders: [
          {text: 'Time', value: 'time', align: 'left'},
          {text: 'Status', value: 'status', align: 'right'},
        ],
        statusHistory: [],
        hostheaders: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'IP address', value: 'ip_adress', align: 'right', sortable: false},
          {text: 'Online', value: 'last_seen', align: 'right', sortable: false}
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
        this.axios.get(this.$serverAddr + '/jobs/' + this.$route.params.id).then((response) => {
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

              const efficiency = (validWorkunits.map(workunit => workunit.cracking_time).reduce((a, b) => a + b)) / (this.data.hosts.length * this.data.cracking_time);

              this.efficiency = (efficiency * 100).toFixed(2) + ' %';
              //console.log("Efectivity:", this.efficiency);
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
          if (operation === "kill") {
            this.axios.get(this.$serverAddr + '/graph/packagesProgress/' + this.$route.params.id).then((response) => {
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
