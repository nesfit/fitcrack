<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-toolbar flat>
      <v-chip>{{ totalItems }}</v-chip>
      <h2 class="ml-4">
        All jobs
        <span v-show="viewHidden">(hidden)</span>
        <span v-show="status || attackType">that</span>
        <span v-show="status">are {{ status }}</span>
        <span v-show="status && attackType">and</span>
        <span v-show="attackType">use {{ attackType }} attack</span>
      </h2>
      <!--
      <v-spacer />
      <v-btn text>
        <v-icon left>
          mdi-pencil
        </v-icon>
        Rename
      </v-btn>
      <v-btn text>
        <v-icon left>
          mdi-delete
        </v-icon>
        Delete
      </v-btn>
      -->
    </v-toolbar>
    <v-text-field
      v-model="search"
      class="px-2 pt-3"
      clearable
      outlined
      prepend-inner-icon="mdi-table-search"
      label="Search"
      single-line
      hide-details
    />
    <div class="d-flex justify-space-between align-center px-4 pt-2">
      <v-switch
        v-model="viewHidden"
        class="mr-4"
        label="View hidden jobs"
        :prepend-icon="viewHidden ? 'mdi-eye-off' : 'mdi-eye'"
      />
      <v-select
        v-model="status"
        class="mr-4"
        :items="jobs_statuses"
        label="Status"
        single-line
        item-text="text"
        item-value="text"
        prepend-icon="mdi-timelapse"
        clearable
        @change="updateList"
      />
      <v-select
        v-model="attackType"
        :items="jobs_types"
        label="Attack type"
        single-line
        item-text="text"
        item-value="text"
        prepend-icon="mdi-crosshairs-gps"
        clearable
        @change="updateList"
      />
    </div>

    <!--
    <v-toolbar
      dense
      flat
    >
      <v-toolbar-items>
        <v-btn
          text
          small
          :disabled="selectedJobs.length == 0"
        >
          <v-icon left>
            mdi-folder-move
          </v-icon>
          Move to bin
        </v-btn>
        <v-btn
          text
          small
          :disabled="selectedJobs.length == 0"
        >
          <v-icon left>
            mdi-delete-empty
          </v-icon>
          Discard
        </v-btn>
      </v-toolbar-items>
    </v-toolbar>
    -->

    <v-data-table
      ref="table"
      v-model="selectedJobs"
      :headers="headers"
      :items="jobs"
      :search="search"
      :options.sync="pagination"
      :server-items-length="totalItems"
      :loading="loading"
      :footer-props="{itemsPerPageOptions: [25,10,50,100], itemsPerPageText:'Jobs per page'}"
      show-select
      fixed-header
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
        <v-tooltip
          v-if="item.host_count > 0"
          top
        >
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
        <v-tooltip
          v-else
          top
        >
          <template v-slot:activator="{ on }">
            <span
              class="warning--text"
              v-on="on"
            >
              <v-icon
                v-if="item.host_count == 0"
                left
                color="warning"
              >
                mdi-alert
              </v-icon>
              no hosts
            </span>
          </template>
          <span>No hosts, open job detail and assign them!</span>
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
        <div class="actionsBtns">
          <v-tooltip top>
            <template v-slot:activator="{ on }">
              <v-btn
                icon
                class="mx-0"
                :disabled="item.status !== '0' || item.host_count == 0"
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
          <v-menu>
            <template v-slot:activator="{ on }">
              <v-btn
                icon
                class="mx-0"
                v-on="on"
              >
                <v-icon>mdi-dots-vertical</v-icon>
              </v-btn>
            </template>
            <v-list>
              <v-list-item 
                v-if="item.status < 10"
                @click="operateJob(item.id, 'restart')"
              >
                <v-list-item-title>
                  <v-icon left>
                    mdi-restart
                  </v-icon>Restart
                </v-list-item-title>
              </v-list-item>
              <!--
              <v-list-item @click="operateJob(item.id, 'duplicate')">
                <v-list-item-title>
                  <v-icon left>
                    mdi-content-copy
                  </v-icon>Duplicate
                </v-list-item-title>
              </v-list-item>
              -->
              <v-list-item @click="hideJob(item.id)">
                <v-list-item-title>
                  <v-icon left>
                    {{ item.deleted ? 'mdi-eye' : 'mdi-eye-off' }}
                  </v-icon> 
                  {{ item.deleted ? 'Stop hiding' : 'Hide' }}
                </v-list-item-title>
              </v-list-item>
            </v-list>
          </v-menu>
        </div>
      </template>
    </v-data-table>
  </div>
</template>

<script>
  export default {
    name: 'JobsView',
    data () {
      return {
        interval: null,
        status: null,
        attackType: null,
        search: '',
        viewHidden: false,
        totalItems: 0,
        pagination: {},
        selectedJobs: [],
        loading: true,
        headers: [
          {
            text: 'Name',
            align: 'start',
            value: 'name'
          },
          {text: 'Attack type', value: 'attack', align: 'end', sortable: false},
          {text: 'Status', value: 'status', align: 'end'},
          {text: 'Progress', value: 'progress', align: 'end'},
          {text: 'Added', value: 'time', align: 'end'},
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
        jobs_types: [
          {
            'text': 'dictionary',
            'code': 0
          },
          {
            'text': 'mask',
            'code': 1
          },
          {
            'text': 'combinator',
            'code': 2
          },
          {
            'text': 'pcfg',
            'code': 3
          }
        ],
        jobs:
          [
          ]
      }
    },
    watch: {
      pagination: {
        handler: 'updateList',
        deep: true
      },
      '$route.name': 'updateList',
      viewHidden: 'updateList'
    },
    mounted () {
      // this.loadJobs()
      this.interval = setInterval(function () {
        this.loadJobs()
      }.bind(this), 15000)
    },
    beforeDestroy: function () {
      clearInterval(this.interval)
    },
    methods: {
      loadJobs () {
        this.axios.get(this.$serverAddr + '/job', {
          params: {
            'page': this.pagination.page,
            'per_page': this.pagination.itemsPerPage,
            'order_by': this.pagination.sortBy[0],
            'descending': this.pagination.sortDesc[0],
            'name': this.search,
            'status': this.status,
            'attack_mode': this.attackType,
            'showDeleted': this.viewHidden
          }
        }).then((response) => {
          this.jobs = response.data.items;
          this.totalItems = response.data.total;
          this.loading = false
        })
      },
      updateList () {
        this.loading = true
        this.loadJobs()
      },
      updateStatus: function (e) {
        this.status = e;
        this.$refs.table.updatePagination({ page: 1, totalItems: this.totalItems })
      },
      progressToPercentage: function (progress) {
        if(progress > 100){
          progress = 100
        }
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
      },
      hideJob: function (id) {
        this.loading = true
        this.axios.delete(this.$serverAddr + '/job/' + id)
        .then((response) => {
          this.loadJobs()
        })
      }
    }
  }
</script>

<style scoped>
.cont {
  height: 100%;

}

  .actionsBtns{
    text-align: right;
  }

  .middle {
    vertical-align: middle;
  }

</style>
