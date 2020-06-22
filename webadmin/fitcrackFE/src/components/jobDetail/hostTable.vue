<template>
  <v-data-table
    :headers="headers"
    :items="hosts"
    hide-default-footer
    no-data-text="None assigned"
  >
    <template v-slot:item.name="{ item }">
      <router-link
        :to="{ name: 'hostDetail', params: { id: item.id}}"
        class="middle"
      >
        {{ item.domain_name + ' (' + fixUserNameEncoding(item.user.name) + ')' }}
      </router-link>
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
import fixUserNameEncoding from '@/assets/scripts/unswedishify'

export default {
  props: {
    hosts: {
      type: Array,
      default: () => []
    }
  },
  data () {
    return {
      headers: [
        {
          text: 'Name',
          align: 'start',
          value: 'name'
        },
        {text: 'IP address', value: 'ip_address', align: 'end', sortable: false},
        {text: 'Online', value: 'last_active', align: 'end', sortable: false}
      ]
    }
  },
  methods: {
    fixUserNameEncoding,
    //
    parseTimeDelta (delta) {
      if (delta !== null && delta !== undefined) {
        return this.$moment.utc(delta).fromNow()
      } else {
        return 'Unknown'
      }
    }
  }
}
</script>