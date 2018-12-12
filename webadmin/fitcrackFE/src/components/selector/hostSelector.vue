<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-data-table
    :headers="headers"
    :items="items"
    :search="search"
    v-model="selected"
    item-key="id"
    :select-all="selectAll"
    @input="updateSelected"
  >
    <template slot="items" slot-scope="props">
      <tr @click="props.selected = !props.selected">
        <td>
          <v-checkbox
            v-model="props.selected"
            primary
            hide-details
          ></v-checkbox>
        </td>

        <td>{{ props.item.domain_name + ' (' + props.item.user.name + ')'}}</td>
        <td class="text-xs-right">{{ props.item.ip_address }}</td>
        <td class="text-xs-right">{{ props.item.os_name }}</td>
        <td class="text-xs-right oneline" :title="props.item.p_model">{{ props.item.p_model }}</td>
        <td class="text-xs-right" >
          <v-icon :title="parseTimeDelta(props.item.last_active.last_seen)" v-bind:class="{
                          'error--text': props.item.last_active.seconds_delta > 61,
                          'success--text': props.item.last_active.seconds_delta < 60 && props.item.last_active.seconds_delta !== null
                        }">fiber_manual_record</v-icon>
        </td>
        <td class="text-xs-right">
          <v-tooltip top>
            <v-btn icon class="mx-0" :to="{name: 'hostDetail', params: { id: props.item.id}}" slot="activator">
              <v-icon color="primary">link</v-icon>
            </v-btn>
            <span>Go to the host page</span>
          </v-tooltip>
        </td>
      </tr>
    </template>
  </v-data-table>
</template>

<script>
  export default {
    name: "hostSelector",
    props: {
      selectAll: {
        type: Boolean,
        default: true
      },
      value: {
        type: Array,
        default: []
      },
      selectAll: {
        type: Boolean,
        default: false
      }
    },
    data() {
      return {
        items: [],
        loading: false,
        search: '',
        selected: [],
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
          {text: 'Link to', value: 'name', sortable: false, align: 'right', width: "1"}
        ]
      }
    },
    mounted() {
      if (!this.selectAll) {
        this.headers.unshift({width: "1"})
      }
      this.getData()
    },
    methods: {
      getData() {
        this.loading = true
        this.axios.get(this.$serverAddr + '/hosts', {
          params: {
            'all': true
          }
        }).then((response) => {
          this.items = response.data.items
          if (this.selectAll) {
            this.selected = this.items
            this.$emit('input', this.selected)
          }
          this.loading = false
        })
      },
      parseTimeDelta: function (delta) {
        if (delta !== null && delta !== undefined) {
          return this.$moment.utc(delta).fromNow()
        } else {
          return 'Unknown'
        }
      },
      updateSelected() {
        this.$emit('input', this.selected)
      }
    },
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
