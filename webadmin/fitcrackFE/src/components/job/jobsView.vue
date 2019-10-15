<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="cont">
    <v-text-field
      class="px-2 pt-3"
      clearable
      outline
      prepend-inner-icon="search"
      label="Search"
      single-line
      hide-details
      v-model="search"
    ></v-text-field>
    <div class="d-flex justify-space-between align-center px-4 pt-2">
      <v-switch
        label="View hidden jobs"
        :prepend-icon="viewHidden ? 'visibility_off' : 'visibility'"
        v-model="viewHidden"
      ></v-switch>
      <v-select
        class="mr-4"
        :items="jobs_statuses"
        label="Status"
        single-line
        item-text="text"
        item-value="text"
        prepend-icon="av_timer"
        clearable
        v-model="status"
        @change="updateList"
      ></v-select>
      <v-select
        :items="jobs_types"
        label="Attack type"
        single-line
        item-text="text"
        item-value="text"
        prepend-icon="gps_fixed"
        clearable
        v-model="attackType"
        @change="updateList"
      ></v-select>
    </div>
    <v-divider></v-divider>
    <v-data-table
      ref="table"
      :headers="headers"
      :items="jobs"
      :search="search"
      :pagination.sync="pagination"
      :total-items="totalItems"
      :loading="loading"
      :rows-per-page-items="[25,10,50,100]"
      rows-per-page-text="Jobs per page"
      disable-initial-sort
    >
      <tr slot="items" slot-scope="props">
        <td>
          <router-link :to="{ name: 'jobDetail', params: { id: props.item.id}}" class="middle">{{ props.item.name }}</router-link>
        </td>
        <td class="text-xs-right">{{ props.item.attack }}</td>
        <td class="text-xs-right" v-bind:class="props.item.status_type + '--text'">
          <v-tooltip top>
            <span
              slot="activator"
            >
              {{ props.item.status_text }}
            </span>
            <span>{{ props.item.status_tooltip }}</span>
          </v-tooltip>
        </td>
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
        <td>
          <div class="actionsBtns">
            <v-tooltip top>
              <v-btn icon class="mx-0"  :disabled="props.item.status !== '0'"  slot="activator" @click="operateJob(props.item.id, 'start')">
                <v-icon color="success">play_circle_outline</v-icon>
              </v-btn>
              <span>Start job</span>
            </v-tooltip>
            <v-tooltip top>
              <v-btn icon class="mx-0"  :disabled="props.item.status !== '10'"  slot="activator" @click="operateJob(props.item.id, 'stop')">
                <v-icon color="error">pause_circle_outline</v-icon>
              </v-btn>
              <span>Stop job</span>
            </v-tooltip>
            <v-menu>
              <template v-slot:activator="{ on }">
                <v-btn
                  icon
                  class="mx-0"
                  v-on="on"
                >
                  <v-icon>more_vert</v-icon>
                </v-btn>
              </template>
              <v-list>
                <v-list-tile 
                  v-if="props.item.status >= 10"
                  @click="operateJob(props.item.id, 'restart')">
                  <v-list-tile-title><v-icon left>loop</v-icon>Restart</v-list-tile-title>
                </v-list-tile>
                <v-list-tile @click="operateJob(props.item.id, 'duplicate')">
                  <v-list-tile-title><v-icon left>content_copy</v-icon>Duplicate</v-list-tile-title>
                </v-list-tile>
                <v-list-tile @click="hideJob(props.item.id)">
                  <v-list-tile-title>
                    <v-icon left>{{props.item.deleted ? 'visibility' : 'visibility_off'}}</v-icon> 
                    {{props.item.deleted ? 'Stop hiding' : 'Hide'}}
                  </v-list-tile-title>
                </v-list-tile>
              </v-list>
            </v-menu>
          </div>
        </td>
      </tr>
    </v-data-table>
  </div>
</template>

<script>
  export default {
    name: 'jobsView',
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
        this.axios.get(this.$serverAddr + '/jobs', {
          params: {
            'page': this.pagination.page,
            'per_page': this.pagination.rowsPerPage,
            'order_by': this.pagination.sortBy,
            'descending': this.pagination.descending,
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
        this.axios.get(this.$serverAddr + '/jobs/' + id + '/action', {
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
        this.axios.delete(this.$serverAddr + '/jobs/' + id)
        .then((response) => {
          this.loadJobs()
        })
      }
    },
    data () {
      return {
        interval: null,
        status: null,
        attackType: null,
        search: '',
        viewHidden: false,
        totalItems: 0,
        pagination: {},
        loading: true,
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Attack type', value: 'attack_mode', align: 'right'},
          {text: 'Status', value: 'status', align: 'right'},
          {text: 'Progress', value: 'progress', align: 'right'},
          {text: 'Added', value: 'time', align: 'right'},
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
    }
  }
</script>

<style scoped>
.cont {
  height: 100%;
  background: white;
}

  .progressPercentage{
    font-size: 11px;
  }

  .actionsBtns{
    text-align: right;
  }

  .middle {
    vertical-align: middle;
  }

</style>

<style>
  .jobProgress .progress-circular__overlay {
    transition: none;
  }
</style>
