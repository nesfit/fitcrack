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
      ></v-text-field>
    </v-card-title>
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
        handler() {
          this.loading = true;
          this.loadHosts()
        },
        deep: true
      },
      '$route.name': {
        handler () {
          this.loading = true;
          this.loadHosts()
        }
      },
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
            'showDeleted': this.$route.name === 'hiddenHosts'
          }
        }).then((response) => {
          this.hosts = response.data.items;
          this.totalItems = response.data.total;
          this.loading = false
        })
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
          {text: 'Online', value: 'last_seen', align: 'right', sortable: false},
          {text: 'Hide', value: 'name', sortable: false, align: 'right', width: "1"}
        ],
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
