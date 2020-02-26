<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-data-table
    v-model="selected"
    :headers="headers"
    :items="items"
    :search="search"
    item-key="id"
    show-select
    :single-select="!selectAll"
    @input="updateSelected"
  >
    <template v-slot:item.domain_name="{ item }">
      <router-link
        :to="{ name: 'hostDetail', params: {id: item.id} }"
        class="middle" target='_blank'
      >
        {{ item.domain_name + ' (' + item.user.name + ')' }}
        <v-icon 
          small
          color="primary"
        >
          mdi-open-in-new
        </v-icon>
      </router-link>
    </template>
    <template v-slot:item.p_model="{ item }">
      <span class="oneline">{{ item.p_model.replace(/(?:\(R\)|\(TM\)|Intel|AMD)/g, '') }}</span>
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
</template>

<script>
  import selector from './selectorMixin'
  export default {
    name: "HostSelector",
    mixins: [selector],
    props: {
      autoRefresh: Boolean
    },
    data() {
      return {
        headers: [
          {
            text: 'Name',
            align: 'start',
            value: 'domain_name'
          },
          {text: 'IP address', value: 'ip_address', align: 'end', sortable: false},
          {text: 'OS', value: 'os_name', align: 'end', sortable: false},
          {text: 'Processor', value: 'p_model', align: 'end', width: '200', sortable: false},
          {text: 'Online', value: 'last_active', align: 'end', sortable: false},
        ]
      }
    },
    mounted () {
      this.interval = setInterval(() => {
        if (this.autoRefresh) this.getData(true)
      }, 2000)
    },
    beforeDestroy () {
      clearInterval(this.interval)
    },
    methods: {
      getData(autorefreshing = false) {
        this.loading = true
        this.axios.get(this.$serverAddr + '/hosts', {
          params: {
            'all': true
          }
        }).then((response) => {
          this.items = response.data.items
          if (!autorefreshing) this.selected = this.items
          this.loading = false
        })
      },
      parseTimeDelta: function (delta) {
        if (delta !== null && delta !== undefined) {
          return this.$moment.utc(delta).fromNow()
        } else {
          return 'Unknown'
        }
      }
    }
  }
</script>

<style scoped>
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
</style>
