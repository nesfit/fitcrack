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
        label="View hidden hosts"
        :prepend-icon="viewHidden ? 'visibility_off' : 'visibility'"
        v-model="viewHidden"
      ></v-switch>
      <v-select
        class="mr-4"
        :items="hosts_statuses"
        label="Online status"
        single-line
        item-text="text"
        item-value="text"
        prepend-icon="power_settings_new"
        clearable
        v-model="status"
        @change="updateList"
      ></v-select>
    </div>
    <v-divider></v-divider>
    <v-data-table
      ref="table"
      :headers="headers"
      :items="hosts"
      :search="search"
      :pagination.sync="pagination"
      :total-items="totalItems"
      :loading="loading"
      :rows-per-page-items="[25,50,100]"
      rows-per-page-text="Hosts per page"
      disable-initial-sort
    >
      <template slot="items" slot-scope="props">
        <td>
          <router-link :to="{ name: 'hostDetail', params: { id: props.item.id}}" class="middle">
            {{ props.item.domain_name + ' (' + props.item.user.name + ')'}}
          </router-link>
        </td>
        <td class="text-xs-right">{{ props.item.ip_address }}</td>
        <td class="text-xs-right">{{ props.item.os_name }}</td>
        <td class="text-xs-right oneline" :title="props.item.p_model">{{ props.item.p_model }}</td>
        <td class="text-xs-right">{{ props.item.jobs.map(j => j.status === 10 ? 1 : 0).reduce((a, b) => a + b, 0) }}</td>
        <td class="text-xs-right"  v-bind:class="{
          'error--text': props.item.last_active.seconds_delta > 61,
          'success--text': props.item.last_active.seconds_delta < 60 && props.item.last_active.seconds_delta !== null
        }">
          <v-icon :title="parseTimeDelta(props.item.last_active.last_seen)" class="inheritColor">fiber_manual_record</v-icon>
        </td>
        <td class="text-xs-right">
          <v-tooltip top v-if="props.item.deleted">
            <v-btn icon class="mx-0" slot="activator" @click="hideJob(props.item.id)">
              <v-icon color="grey darken-3 ">visibility</v-icon>
            </v-btn>
            <span>Show host</span>
          </v-tooltip>
          <v-tooltip top v-else>
            <v-btn icon class="mx-0" slot="activator" @click="hideJob(props.item.id)">
              <v-icon color="grey ">visibility_off</v-icon>
            </v-btn>
            <span>Hide host</span>
          </v-tooltip>
        </td>


      </template>
    </v-data-table>
  </div>
</template>

<script>
  export default {
    name: "hostsView",
    watch: {
      pagination: {
        handler: 'updateList',
        deep: true
      },
      '$route.name': 'updateList',
      viewHidden: 'updateList'
    },
    mounted() {
      this.interval = setInterval(function () {
        this.loadHosts()
      }.bind(this), 5000)
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
      loadHosts() {
        this.axios.get(this.$serverAddr + '/hosts', {
          params: {
            'page': this.pagination.page,
            'per_page': this.pagination.rowsPerPage,
            'order_by': this.pagination.sortBy,
            'descending': this.pagination.descending,
            'name': this.search,
            'showDeleted': this.viewHidden
          }
        }).then((response) => {
          this.hosts = response.data.items;
          this.totalItems = response.data.total;
          this.loading = false
        })
      },
      updateList () {
        this.loading = true
        this.loadHosts()
      },
      hideJob: function (id) {
        this.loading = true
        this.axios.delete(this.$serverAddr + '/hosts/' + id)
          .then((response) => {
            this.loadHosts()
          })
      }
    },
    data: function () {
      return {
        interval: null,
        status: 'active',
        search: '',
        viewHidden: false,
        totalItems: 0,
        pagination: {},
        loading: true,
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'domain_name'
          },
          {text: 'IP address', value: 'ip_adress', align: 'right', sortable: false},
          {text: 'Operating system', value: 'os_name', align: 'right', sortable: false},
          {text: 'Processor', value: 'p_model', align: 'right', width: '200', sortable: false},
          {text: 'Active jobs', value: 'jobs', align: 'right', sortable: false},
          {text: 'Online', value: 'last_seen', align: 'right', sortable: false},
          {text: 'Show or hide', value: 'name', sortable: false, align: 'right', width: "1"}
        ],
        hosts_statuses: [],
        hosts:
          []
      }
    }
  }
</script>

<style scoped>

  .middle {
    vertical-align: middle;
  }

  .cont {
    height: 100%;
    background: white;
  }

  .oneline {
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
    display: block;
    width: 200px;
    vertical-align: middle;
    line-height: 50px;
    height: 50px;
  }

  .inheritColor {
    color: inherit !important;
  }

</style>
