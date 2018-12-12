<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="height100">
    <v-navigation-drawer
      persistent
      class="navigationDrawer"
      :mini-variant="miniVariant"
      :clipped="clipped"
      v-model="drawer"
      enable-resize-watcher
      fixed
      app
      color="primary"
    >
      <router-link :to="{ name: 'home'}">
        <img :src="require('@/assets/fitcrack.png')" class="mx-auto px-2 mt-2 d-block logo" alt="logo"/>
        <h2 v-show="!miniVariant" class="logoText" ></h2>
      </router-link>
      <v-divider></v-divider>
      <v-list expand>
        <v-list-tile to="/">
          <v-list-tile-action>
            <v-icon>home</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Home</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-group :value="true">
          <v-list-tile :to="{ name: 'jobs'}" slot="activator" @click.stop="">
            <v-list-tile-action>
              <v-icon>work</v-icon>
            </v-list-tile-action>
            <v-list-tile-content>
              <v-list-tile-title>Jobs</v-list-tile-title>
            </v-list-tile-content>
          </v-list-tile>
          <v-list-tile :to="{ name: 'hiddenJobs'}">
            <v-list-tile-action>
              <v-icon>visibility_off</v-icon>
            </v-list-tile-action>
            <v-list-tile-content>
              <v-list-tile-title>Hidden jobs</v-list-tile-title>
            </v-list-tile-content>
          </v-list-tile>
          <v-list-tile :to="{ name: 'addJob'}" v-if="$userCanAddJob()">
            <v-list-tile-action>
              <v-icon>add</v-icon>
            </v-list-tile-action>
            <v-list-tile-content>
              <v-list-tile-title>Add job</v-list-tile-title>
            </v-list-tile-content>
          </v-list-tile>
        </v-list-group>

        <v-list-group :value="true">
          <v-list-tile :to="{ name: 'hosts'}" slot="activator" @click.stop="">
            <v-list-tile-action>
              <v-icon>desktop_windows</v-icon>
            </v-list-tile-action>
            <v-list-tile-content>
              <v-list-tile-title>Hosts</v-list-tile-title>
            </v-list-tile-content>
          </v-list-tile>
          <v-list-tile :to="{ name: 'hiddenHosts'}">
            <v-list-tile-action>
              <v-icon>visibility_off</v-icon>
            </v-list-tile-action>
            <v-list-tile-content>
              <v-list-tile-title>Hidden hosts</v-list-tile-title>
            </v-list-tile-content>
          </v-list-tile>
        </v-list-group>
        <v-list-tile :to="{ name: 'hashes'}">
          <v-list-tile-action>
            <database-icon class="material-icons"/>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Hashes</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile :to="{ name: 'dictionaries'}">
          <v-list-tile-action>
            <v-icon>format_align_left</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Dictionaries</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile :to="{ name: 'rules'}">
          <v-list-tile-action>
            <v-icon>gavel</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Rules</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile :to="{ name: 'charsets'}">
          <v-list-tile-action>
            <v-icon>translate</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Charsets</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile :to="{ name: 'masks'}">
          <v-list-tile-action>
            <mask-icon class="material-icons"/>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Masks</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <!--<v-list-tile :to="{ name: 'files'}">-->
          <!--<v-list-tile-action>-->
            <!--<v-icon>insert_drive_file</v-icon>-->
          <!--</v-list-tile-action>-->
          <!--<v-list-tile-content>-->
            <!--<v-list-tile-title>Encrypted files</v-list-tile-title>-->
          <!--</v-list-tile-content>-->
        <!--</v-list-tile>-->
        <v-list-tile :to="{ name: 'markovChains'}">
          <v-list-tile-action>
            <matrix-icon class="material-icons"/>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Markov chains</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <!--<v-list-tile :to="{ name: 'server'}">-->
          <!--<v-list-tile-action>-->
            <!--<v-icon>settings</v-icon>-->
          <!--</v-list-tile-action>-->
          <!--<v-list-tile-content>-->
            <!--<v-list-tile-title>Control</v-list-tile-title>-->
          <!--</v-list-tile-content>-->
        <!--</v-list-tile>-->
        <v-list-tile :to="{ name: 'manageUsers'}" v-if="$userCanManageUsers()">
          <v-list-tile-action>
            <v-icon>group</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Manage users</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-list-tile @click="logout">
          <v-list-tile-action>
            <v-icon>exit_to_app</v-icon>
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title>Logout</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <!--<v-list-group prepend-icon>-->
          <!--<v-list-tile :to="{ name: 'user'}" slot="activator">-->
            <!--<v-list-tile-action>-->
              <!--<v-icon>person</v-icon>-->
            <!--</v-list-tile-action>-->
            <!--<v-list-tile-content>-->
              <!--<v-list-tile-title>User</v-list-tile-title>-->
            <!--</v-list-tile-content>-->
          <!--</v-list-tile>-->
          <!--<v-list-tile :to="{ name: 'manageUsers'}" v-if="$userCanManageUsers()">-->
            <!--<v-list-tile-action>-->
              <!--<v-icon>group</v-icon>-->
            <!--</v-list-tile-action>-->
            <!--<v-list-tile-content>-->
              <!--<v-list-tile-title>Manage users</v-list-tile-title>-->
            <!--</v-list-tile-content>-->
          <!--</v-list-tile>-->
          <!--<v-list-tile @click="logout">-->
            <!--<v-list-tile-action>-->
              <!--<v-icon>exit_to_app</v-icon>-->
            <!--</v-list-tile-action>-->
            <!--<v-list-tile-content>-->
              <!--<v-list-tile-title>Logout</v-list-tile-title>-->
            <!--</v-list-tile-content>-->
          <!--</v-list-tile>-->
        <!--</v-list-group>-->
      </v-list>
    </v-navigation-drawer>
    <v-toolbar
      app
      :clipped-left="clipped"
      height="64px"
      class="mainToolbar"
      color="primary"
      dark
    >
      <v-toolbar-side-icon @click.stop="drawer = !drawer"></v-toolbar-side-icon>
      <v-btn icon @click.stop="miniVariant = !miniVariant">
        <v-icon v-html="miniVariant ? 'chevron_right' : 'chevron_left'"></v-icon>
      </v-btn>
      <v-btn icon @click.stop="clipped = !clipped">
        <v-icon>web</v-icon>
      </v-btn>
      <v-spacer>
        <router-link :to="{ name: 'home'}" class="textLogo">
          <img :src="require('@/assets/fitcrack_white.png')" class="mx-2 logoSmall" alt="logo"/>
          <span class="logoSmallText" v-text="$vuetify.theme.project"></span>

        </router-link>
      </v-spacer>
      <v-badge color="red" overlap>
        <span slot="badge" v-if="notificationsCount > 0">{{notificationsCount}}</span>
        <v-btn icon @click.stop="toggleNotifications" class="ma-0">
          <v-icon>notifications</v-icon>
        </v-btn>
      </v-badge>

    </v-toolbar>
    <v-content class="height100 main">
      <router-view/>
    </v-content>
    <v-navigation-drawer
      class="pa-0"
      temporary
      right
      v-model="rightDrawer"
      fixed
      app
    >
      <v-toolbar class="notifHeader">
        <v-toolbar-title>Notifications</v-toolbar-title>
      </v-toolbar>
      <notifications-wrapper ref="notifWrapper"></notifications-wrapper>
    </v-navigation-drawer>
  </div>
</template>

<script>
  import notifications from '@/components/notification/fc_notifications_wrapper'
  import DBicon from 'vue-material-design-icons/database.vue'
  import maskIcon from 'vue-material-design-icons/guy-fawkes-mask.vue'
  import matrixIcon from "vue-material-design-icons/matrix.vue"


  export default {
    components: {
      'notifications-wrapper': notifications,
      'database-icon': DBicon,
      'mask-icon': maskIcon,
      'matrix-icon': matrixIcon
    },
    mounted: function () {
      if (!this.$store.user.loggedIn) {
        this.$router.push({
          name: 'login'
        })
      }
      this.getNotificationsCount();
      this.interval = setInterval(function () {
        this.getNotificationsCount()
      }.bind(this), 10000)
    },
    beforeDestroy: function () {
      clearInterval(this.interval)
    },
    methods: {
      logout: function () {
        this.axios.get(this.$serverAddr + '/user/logout').then((response) => {
          this.$logoutUser()
        })
      },
      toggleNotifications: function () {
        this.rightDrawer = !this.rightDrawer;
        if (this.rightDrawer) {
          this.$refs.notifWrapper.loadNotifications()
        }
      },
      getNotificationsCount: function () {
        this.axios.get(this.$serverAddr + '/notifications/count', {
          withCredentials: true
        }).then((response) => {
          this.notificationsCount = response.data.count
        })
      }
    },
    data() {
      return {
        clipped: false,
        drawer: true,
        miniVariant: false,
        rightDrawer: false,
        notificationsCount: 0
      }
    }
  }
</script>

<style scoped>
  .notifHeader {
    z-index: 2;
  }

  .logo {
    width: 100%;
    max-width: 150px;
  }

  .logoText {
    color: #4c4c4c;
    text-align: center;
    padding-bottom: 15px;
    font-size: 26px;
    font-weight: 300;
    text-transform: uppercase;
  }

  .textLogo {
    width: 100%;
    display: block;
    color: inherit;
    text-align: center;
  }

  .textLogoText {
    font-size: 18px;
    font-width: 300;
  }

  .height100 {
    height: 100%;
  }

  .mainToolbar {
    z-index: 3;
  }

  .navigationDrawer {
    z-index: 6;
  }

  .main {
    padding-top: 64px !important;
  }

  .logoSmall {
    width: 100%;
    max-width: 40px;
    vertical-align: middle;
  }

  .logoSmallText {
    font-size: 24px;
    text-transform: uppercase;
    font-weight: 300;
    vertical-align: middle;
  }
</style>


<style>
  .v-list__group__items .v-list__tile {
    padding-left: 35px;
  }

</style>
