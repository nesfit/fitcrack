<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="height100">
    <v-navigation-drawer
      v-model="drawer"
      persistent
      class="navigationDrawer"
      enable-resize-watcher
      fixed
      app
    >
      <router-link :to="{ name: 'home'}">
        <img
          v-if="$vuetify.theme.dark"
          :src="require('@/assets/fitcrack-glow.svg')"
          class="mx-auto px-2 mt-2 d-block logo"
          alt="logo"
        >
        <img
          v-else
          :src="require('@/assets/fitcrack.svg')"
          class="mx-auto px-2 mt-2 d-block logo"
          alt="logo"
        >
        <h2 class="logoText" />
      </router-link>

      <v-divider />

      <v-list
        expand
        nav
      >
        <v-list-item to="/">
          <v-list-item-action>
            <v-icon>mdi-home</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Home</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-group
          :value="true"
          prepend-icon="mdi-briefcase-outline"
        >
          <template v-slot:activator>
            <v-list-item-title>Jobs</v-list-item-title>
          </template>

          <v-list-item
            v-if="$userCanAddJob()"
            :to="{ name: 'addJob'}"
          >
            <v-list-item-action>
              <v-icon>mdi-briefcase-plus</v-icon>
            </v-list-item-action>
            <v-list-item-content>
              <v-list-item-title>Add job</v-list-item-title>
            </v-list-item-content>
          </v-list-item>

          <v-list-item :to="{ name: 'jobs'}">
            <v-list-item-action>
              <v-icon>mdi-briefcase</v-icon>
            </v-list-item-action>
            <v-list-item-content>
              <v-list-item-title>All jobs</v-list-item-title>
            </v-list-item-content>
          </v-list-item>

          <!--
          <v-list-item :to="{ name: 'hiddenJobs'}">
            <v-list-item-action>
              <v-icon>visibility_off</v-icon>
            </v-list-item-action>
            <v-list-item-content>
              <v-list-item-title>Hidden jobs</v-list-item-title>
            </v-list-item-content>
          </v-list-item>
          -->

          <v-divider class="mb-2" />
        </v-list-group>

        <v-list-item :to="{ name: 'hosts'}">
          <v-list-item-action>
            <v-icon>mdi-desktop-classic</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Hosts</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'hashes'}">
          <v-list-item-action>
            <v-icon>mdi-pound</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Hashes</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'dictionaries'}">
          <v-list-item-action>
            <v-icon>mdi-dictionary</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Dictionaries</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'pcfg'}">
          <v-list-item-action>
            <v-icon>mdi-ray-start-end</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>PCFG</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'rules'}">
          <v-list-item-action>
            <v-icon>mdi-gavel</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Rules</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'charsets'}">
          <v-list-item-action>
            <v-icon>mdi-alphabetical</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Charsets</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'masks'}">
          <v-list-item-action>
            <v-icon>mdi-guy-fawkes-mask</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Masks</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <!--<v-list-item :to="{ name: 'files'}">-->
        <!--<v-list-item-action>-->
        <!--<v-icon>insert_drive_file</v-icon>-->
        <!--</v-list-item-action>-->
        <!--<v-list-item-content>-->
        <!--<v-list-item-title>Encrypted files</v-list-item-title>-->
        <!--</v-list-item-content>-->
        <!--</v-list-item>-->

        <v-list-item :to="{ name: 'markovChains'}">
          <v-list-item-action>
            <v-icon>mdi-matrix</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Markov chains</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <!--<v-list-item :to="{ name: 'server'}">-->
        <!--<v-list-item-action>-->
        <!--<v-icon>settings</v-icon>-->
        <!--</v-list-item-action>-->
        <!--<v-list-item-content>-->
        <!--<v-list-item-title>Control</v-list-item-title>-->
        <!--</v-list-item-content>-->
        <!--</v-list-item>-->

        <v-divider />

        <v-list-item
          v-if="$userCanManageUsers()"
          :to="{ name: 'manageUsers'}"
        >
          <v-list-item-action>
            <v-icon>mdi-folder-account</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Manage users</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'myAccount'}">
          <v-list-item-action>
            <v-icon>mdi-account</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>My account</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item
          v-if="$userCanManageUsers()"
          :to="{ name: 'server'}"
        >
          <v-list-item-action>
            <v-icon>mdi-server</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Server</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item @click="logout">
          <v-list-item-action>
            <v-icon>mdi-logout</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Logout</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <!--<v-list-group prepend-icon>-->
        <!--<v-list-item :to="{ name: 'user'}" slot="activator">-->
        <!--<v-list-item-action>-->
        <!--<v-icon>person</v-icon>-->
        <!--</v-list-item-action>-->
        <!--<v-list-item-content>-->
        <!--<v-list-item-title>User</v-list-item-title>-->
        <!--</v-list-item-content>-->
        <!--</v-list-item>-->
        <!--<v-list-item :to="{ name: 'manageUsers'}" v-if="$userCanManageUsers()">-->
        <!--<v-list-item-action>-->
        <!--<v-icon>group</v-icon>-->
        <!--</v-list-item-action>-->
        <!--<v-list-item-content>-->
        <!--<v-list-item-title>Manage users</v-list-item-title>-->
        <!--</v-list-item-content>-->
        <!--</v-list-item>-->
        <!--<v-list-item @click="logout">-->
        <!--<v-list-item-action>-->
        <!--<v-icon>exit_to_app</v-icon>-->
        <!--</v-list-item-action>-->
        <!--<v-list-item-content>-->
        <!--<v-list-item-title>Logout</v-list-item-title>-->
        <!--</v-list-item-content>-->
        <!--</v-list-item>-->
        <!--</v-list-group>-->
      </v-list>
    </v-navigation-drawer>

    <!-- TOP BAR -->

    <v-app-bar
      app
      height="64px"
      class="mainToolbar"
    >
      <v-app-bar-nav-icon @click.stop="drawer = !drawer" />
      <v-toolbar-title>{{ $store.project }}</v-toolbar-title>
      <!--
      <router-link :to="{ name: 'home'}" class="textLogo">
        <span class="logoSmallText" v-text="$vuetify.theme.project"></span>
      </router-link>
      -->
      <v-spacer />
      <v-btn
        icon
        @click.stop="$vuetify.theme.dark = !$vuetify.theme.dark"
      >
        <v-icon>mdi-theme-light-dark</v-icon>
      </v-btn>
      <v-badge
        color="red"
        overlap
      >
        <span
          v-if="notificationsCount > 0"
          slot="badge"
        >{{ notificationsCount }}</span>
        <v-btn
          icon
          class="ma-0"
          @click.stop="toggleNotifications"
        >
          <v-icon>{{ notificationsCount > 0 ? 'mdi-bell-ring' : 'mdi-bell' }}</v-icon>
        </v-btn>
      </v-badge>
    </v-app-bar>
    <v-content class="height100 main">
      <router-view />
    </v-content>
    <v-navigation-drawer
      v-model="rightDrawer"
      class="pa-0"
      temporary
      right
      fixed
      app
    >
      <v-toolbar
        flat 
        class="notifHeader"
      >
        <v-toolbar-title>Notifications</v-toolbar-title>
      </v-toolbar>
      <notifications-wrapper ref="notifWrapper" />
    </v-navigation-drawer>
  </div>
</template>

<script>
  import notifications from '@/components/notification/fc_notifications_wrapper'

  export default {
    components: {
      'notifications-wrapper': notifications
    },
    data() {
      return {
        drawer: true,
        rightDrawer: false,
        notificationsCount: 0
      }
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
    // width: 100%;
    // display: block;
    color: inherit;
    // text-align: center;
  }

  .textLogoText {
    font-size: 18px;
    font-weight: 300;
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
