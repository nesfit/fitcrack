<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs
      v-if="data != null"
      :items="
        [
          { text: 'Hosts', to: { name : 'hosts' }, exact: true },
          { text: data.domain_name }
        ]"
      divider="/"
    />

    <v-container>
      <fc-tile
        title="Host info"
        :loading="data==null"
        class=" dictContentContainer mb-5"
      >
        <v-list
          v-if="data != null"
          single-line
          class="width100"
        >
          <v-list-item class="px-2 py-1">
            <v-list-item-action class="pr-3 key">
              Name:
            </v-list-item-action>
            <v-list-item-content>
              <v-list-item-title class="text-right">
                {{ data.domain_name }}
              </v-list-item-title>
            </v-list-item-content>
          </v-list-item>
          <v-divider />
          <v-list-item class="px-2 py-1">
            <v-list-item-action class="pr-3 key">
              User:
            </v-list-item-action>
            <v-list-item-content>
              <v-list-item-title class="text-right">
                {{ data.user.name }}
              </v-list-item-title>
            </v-list-item-content>
          </v-list-item>
          <v-divider />
          <v-list-item class="px-2 py-1">
            <v-list-item-action class="pr-3 key">
              Operation system:
            </v-list-item-action>
            <v-list-item-content>
              <v-list-item-title class="text-right">
                {{ data.os_name }}
              </v-list-item-title>
            </v-list-item-content>
          </v-list-item>
          <v-divider />
          <v-list-item class="px-2 py-1">
            <v-list-item-action class="pr-3 key">
              Processor:
            </v-list-item-action>
            <v-list-item-content>
              <v-list-item-title class="text-right">
                {{ data.p_model }}
              </v-list-item-title>
            </v-list-item-content>
          </v-list-item>
        </v-list>
        <v-divider />
      </fc-tile>


      <fc-tile
        title="Jobs"
        class="max800 mb-5"
        :loading="data === null"
      >
        <v-data-table
          v-if="data !== null"
          :headers="jobHeaders"
          :items="data.jobs"
          :footer-props="{itemsPerPageOptions: [10,25,50,100], itemsPerPageText: 'Jobs per page'}"
        >
          <template
            slot="items"
            slot-scope="props"
          >
            <router-link
              :to="{ name: 'jobDetail', params: { id: props.item.id}}"
              class="middle"
            >
              <td>{{ props.item.name }}</td>
            </router-link>
            <td class="text-right">
              {{ props.item.attack }}
            </td>
            <td
              class="text-right"
              :class="props.item.status_type + '--text'"
            >
              {{ props.item.status_text }}
            </td>
            <td class="text-right">
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
            <td class="text-right">
              {{ $moment(props.item.time).format('D.M.YYYY H:mm:ss') }}
            </td>
            <td class="text-right">
              {{ props.item.password }}
            </td>
            <td>
              <div class="d-flex text-right actionsBtns">
                <v-tooltip top>
                  <template v-slot:activator="{ on }">
                    <v-btn
                      icon
                      class="mx-0"
                      :disabled=" props.item.status !== '0'"
                      v-on="on"
                      @click="operateJob(props.item.id, 'start')"
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
                      :disabled=" props.item.status >= 10"
                      v-on="on"
                      @click="operateJob(props.item.id, 'restart')"
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
                      :disabled=" props.item.status !== '10'"
                      v-on="on"
                      @click="operateJob(props.item.id, 'stop')"
                    >
                      <v-icon color="error">
                        pause_circle_outlined
                      </v-icon>
                    </v-btn>
                  </template>
                  <span>Stop job</span>
                </v-tooltip>
              </div>
            </td>
          </template>
        </v-data-table>
      </fc-tile>



      <fc-tile
        title="Workunits"
        class="max800 mb-5"
        :loading="data === null"
      >
        <v-data-table
          v-if="data !== null"
          :footer-props="{itemsPerPageOptions: [10,30,60,{'text':'All','value':-1}], itemsPerPageText: 'Workunits per page'}"
          :headers="workunitsHeader"
          :items="data.workunits"
          class="width100"
        >
          <template
            slot="items"
            slot-scope="props"
          >
            <router-link
              :to="{ name: 'jobDetail', params: { id: props.item.job_id}}"
              class="middle"
            >
              <td>{{ props.item.job.name }}</td>
            </router-link>
            <td>{{ props.item.cracking_time_str }}</td>
            <td class="text-right">
              {{ $moment(props.item.time).format('D.M.YYYY H:mm:ss') }}
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
          </template>
        </v-data-table>
      </fc-tile>
    </v-container>
  </div>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "HostDetailView",
    components: {
      'fc-tile': tile
    },
    data: function () {
      return {
        data: null,
        workunitsHeader: [
          {text: 'Job', align: 'left', value: 'job_id'},
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
