<template>
  <div>
    <!-- name -->
    <v-list-item>
      <v-list-item-content>
        <v-text-field
          v-model="edit.name"
          outlined
          hide-details
        >
          <template #label>
            <v-icon class="label-icon">
              mdi-format-title
            </v-icon>
            Name
          </template>
        </v-text-field>
      </v-list-item-content>
    </v-list-item>
    <!-- job start -->
    <v-list-item>
      <v-list-item-content class="py-0">
        <v-list-item-title class="d-flex align-center">
          <div class="subtitle-2">
            <v-icon>
              mdi-ray-start
            </v-icon>
            Start
          </div>
          <v-spacer />
          <v-checkbox
            v-model="edit.startNow"
            label="No Start Delay"
          />
        </v-list-item-title>
        <picker
          v-show="!edit.startNow"
          v-model="edit.time_start"
          outlined
          hide-details
        />
      </v-list-item-content>
    </v-list-item>
    <!-- job end -->
    <v-list-item>
      <v-list-item-content class="pt-0">
        <v-list-item-title class="d-flex align-center">
          <div class="subtitle-2">
            <v-icon>
              mdi-ray-end
            </v-icon>
            End
          </div>
          <v-spacer />
          <v-checkbox
            v-model="edit.endNever"
            label="No Time Limit"
          />
        </v-list-item-title>
        <picker
          v-show="!edit.endNever"
          v-model="edit.time_end"
          outlined
          hide-details
        />
      </v-list-item-content>
    </v-list-item>
    <!-- wu secs -->
    <v-list-item>
      <v-list-item-content>
        <v-text-field
          v-model="edit.seconds_per_job"
          outlined
          hide-details
          type="number"
          suffix="seconds"
        >
          <template #label>
            <v-icon class="label-icon">
              mdi-timeline-clock
            </v-icon>
            Time per Workunit
          </template>
        </v-text-field>
      </v-list-item-content>
    </v-list-item>
    <!-- comment -->
    <v-list-item>
      <v-list-item-content>
        <v-textarea
          v-model="edit.comment"
          outlined
          auto-grow
          hide-details
        >
          <template #label>
            <v-icon class="label-icon">
              mdi-format-quote-close
            </v-icon>
            Comment
          </template>
        </v-textarea>
      </v-list-item-content>
    </v-list-item>
    <!-- save button -->
    <v-list-item>
      <v-list-item-content>
        <v-btn
          text
          @click="save"
        >
          <v-icon left>
            mdi-content-save
          </v-icon>
          <span>Save</span>
        </v-btn>
      </v-list-item-content>
    </v-list-item>
  </div>
</template>

<script>
import Picker from '@/components/picker/datetime'

export default {
  components: {
    Picker
  },
  props: {
    data: {
      type: Object,
      default: () => {}
    }
  },
  data () {
    return {
      edit: {
        name: this.data.name,
        comment: this.data.comment,
        seconds_per_job: this.data.seconds_per_job,
        time_start: this.data.time_start === null ?
        this.$moment().toISOString(true).slice(0, 16) :
        this.$moment(this.data.time_start).toISOString(true).slice(0, 16),
        time_end: this.data.time_end === null ?
          this.$moment().toISOString(true).slice(0, 16) :
          this.$moment(this.data.time_end).toISOString(true).slice(0, 16),
        startNow: (this.data.time_start === null),
        endNever: (this.data.time_end === null)
      }
    }
  },
  methods: {
    save () {
      if (this.edit.startNow) {
        this.edit.time_start = ''
      }

      if (this.edit.endNever) {
        this.edit.time_end = ''
      }

      this.axios.put(this.$serverAddr + '/job/' + this.data.id , this.edit
      ).then((response) => {
        this.$emit('save')
      })
    }
  }
}
</script>

<style scoped>
.label-icon {
  color:inherit;
  transition:none;
}
</style>