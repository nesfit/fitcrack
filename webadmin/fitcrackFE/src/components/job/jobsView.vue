<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="cont">
    <v-card-title class="pt-2 pb-1">
      <v-text-field
        clearable
        solo
        flat
        append-icon="search"
        label="Search by name"
        single-line
        hide-details
        v-model="search"
      > </v-text-field>
      <v-spacer></v-spacer>
      <v-select
        :items="jobs_statuses"
        label="Filter by status"
        single-line
        item-text="text"
        item-value="text"
        solo
        flat
        clearable
        @change="updateStatus"
      > </v-select>
    </v-card-title>
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
          <div class="d-flex text-xs-right actionsBtns">
            <v-tooltip top>
              <v-btn icon class="mx-0"  :disabled="props.item.status !== '0'"  slot="activator" @click="operateJob(props.item.id, 'start')">
                <v-icon color="success">play_circle_outline</v-icon>
              </v-btn>
              <span>Start job</span>
            </v-tooltip>
            <v-tooltip top>
              <v-btn icon class="mx-0"  :disabled="props.item.status >= 10"  slot="activator" @click="operateJob(props.item.id, 'restart')">
                <v-icon color="info">loop</v-icon>
              </v-btn>
              <span>Restart job</span>
            </v-tooltip>
            <v-tooltip top>
              <v-btn icon class="mx-0"  :disabled="props.item.status !== '10'"  slot="activator" @click="operateJob(props.item.id, 'stop')">
                <v-icon color="error">pause_circle_outline</v-icon>
              </v-btn>
              <span>Stop job</span>
            </v-tooltip>
          </div>
        </td>
        <td class="text-xs-right">
          <v-tooltip top v-if="props.item.deleted">
            <v-btn icon class="mx-0" slot="activator" @click="hideJob(props.item.id)">
              <v-icon color="grey darken-3 ">visibility</v-icon>
            </v-btn>
            <span>Show job</span>
          </v-tooltip>
          <v-tooltip top v-else>
            <v-btn icon class="mx-0" slot="activator" @click="hideJob(props.item.id)">
              <v-icon color="grey ">visibility_off</v-icon>
            </v-btn>
            <span>Hide job</span>
          </v-tooltip>
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
        handler () {
          this.loading = true;
          this.loadJobs()
        },
        deep: true
      },
      '$route.name': {
        handler () {
          this.loading = true;
          this.loadJobs()
        }
      },
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
            'showDeleted': this.$route.name === 'hiddenJobs'
          }
        }).then((response) => {
          this.jobs = response.data.items;
          this.totalItems = response.data.total;
          this.loading = false
        })
      },
      updateStatus: function (e) {
        this.status = e;
        this.$refs.table.updatePagination({ page: 1, totalItems: this.totalItems })
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
        search: '',
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
          {text: 'Actions', value: 'name', sortable: false, align: 'right'},
          {text: 'Hide', value: 'name', sortable: false, align: 'right', width: "1"}
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
    width: 125px;
    float: right;
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
