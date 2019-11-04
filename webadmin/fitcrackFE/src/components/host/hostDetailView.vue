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
        class=" max800 mb-5"
      >
        <v-list v-if="data != null">
          <v-list-item>
            <v-list-item-icon>
              <v-icon>mdi-desktop-classic</v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title>
                {{ data.domain_name }}
              </v-list-item-title>
              <v-list-item-subtitle>
                User: {{ data.user.name }}
              </v-list-item-subtitle>
            </v-list-item-content>
          </v-list-item>
          <v-list-item>
            <v-list-item-icon>
              <v-icon>{{ getOsIcon(data.os_name) }}</v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title>
                {{ data.os_name }}
              </v-list-item-title>
            </v-list-item-content>
          </v-list-item>
          <v-list-item>
            <v-list-item-icon>
              <v-icon>mdi-memory</v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title>
                {{ data.p_model }}
              </v-list-item-title>
            </v-list-item-content>
          </v-list-item>
        </v-list>
      </fc-tile>


      <fc-tile
        title="Jobs"
        class="mb-5"
        :loading="data === null"
      >
        <v-data-table
          v-if="data !== null"
          :headers="jobHeaders"
          :items="data.jobs"
          :footer-props="{itemsPerPageOptions: [10,25,50,100], itemsPerPageText: 'Jobs per page'}"
        >
          <!-- Job name cell -->
          <template v-slot:item.name="{ item }">
            <router-link
              :to="{ name: 'jobDetail', params: { id: item.id } }"
              class="middle"
            >
              {{ item.name }}
            </router-link>
          </template>
          <!-- Status text cell -->
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
          <!-- Progress indicator cell -->
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
          <!-- Date added cell -->
          <template v-slot:item.time="{ item }">
            {{ $moment(item.time).format('D.M.YYYY H:mm:ss') }}
          </template>
          <!-- Action buttons cell -->
          <template v-slot:item.actions="{ item }">
            <div class="d-flex justify-end actionsBtns">
              <v-tooltip top>
                <template v-slot:activator="{ on }">
                  <v-btn
                    icon
                    class="mx-0"
                    :disabled="item.status !== '0'"
                    v-on="on"
                    @click="operateJob(item.id, 'start')"
                  >
                    <v-icon color="success">
                      mdi-play-circle
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
                    :disabled="item.status !== '10'"
                    v-on="on"
                    @click="operateJob(item.id, 'stop')"
                  >
                    <v-icon color="error">
                      mdi-pause-circle
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
                    :disabled="item.status >= 10"
                    v-on="on"
                    @click="operateJob(item.id, 'restart')"
                  >
                    <v-icon color="info">
                      mdi-restart
                    </v-icon>
                  </v-btn>
                </template>
                <span>Restart job</span>
              </v-tooltip>
            </div>
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
          <template v-slot:item.job_id="{ item }">
            <router-link
              :to="{ name: 'jobDetail', params: { id: item.job_id}}"
              class="middle"
            >
              {{ item.job.name || 'Detail' }}
            </router-link>
          </template>
          <template v-slot:item.time="{ item }">
            {{ $moment(item.time).format('D.M.YYYY H:mm:ss') }}
          </template>
          <template v-slot:item.retry="{ item }">
            {{ yesNo(item.retry) }}
          </template>
          <template v-slot:item.finished="{ item }">
            {{ yesNo(item.finished) }}
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
          {text: 'Job', align: 'start', value: 'job_id'},
          {text: 'Cracking time', align: 'start', value: 'cracking_time_str'},
          {text: 'Generated', align: 'end', value: 'time'},
          {text: 'Start index', align: 'end', value: 'start_index'},
          {text: 'Keyspace', align: 'end', value: 'hc_keyspace'},
          {text: 'Retry', align: 'end', value: 'retry'},
          {text: 'Finish', align: 'end', value: 'finished'}
        ],
        jobHeaders: [
          {
            text: 'Name',
            align: 'start',
            value: 'name'
          },
          {text: 'Attack type', value: 'attack', align: 'end'},
          {text: 'Status', value: 'status', align: 'end'},
          {text: 'Progress', value: 'progress', align: 'end'},
          {text: 'Added', value: 'time', align: 'end'},
          {text: 'Result', value: 'password', align: 'end', sortable: false},
          {text: 'Actions', value: 'actions', sortable: false, align: 'end'}
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
      getOsIcon (os) {
        if (/(windows|microsoft)/ig.test(os)) {
          return 'mdi-windows'
        } else if (/(linux)/ig.test(os)) {
          return 'mdi-linux'
        } else if (/(apple|mac)/ig.test(os)) {
          return 'mdi-apple'
        } else {
          return 'mdi-application'
        }
      },
      progressToPercentage: function (progress) {
        return progress.toFixed() + '%'
      },
      operateJob: function (id, operation) {
        this.axios.get(this.$serverAddr + '/job/' + id + '/action', {
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
