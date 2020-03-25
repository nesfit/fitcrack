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
        dense
      >
        <v-list-item to="/">
          <v-list-item-action>
            <v-icon>{{ routeIcon('home') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Dashboard</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item
          v-if="$store.getters.can('ADD_NEW_JOB')"
          :to="{ name: 'addJob'}"
        >
          <v-list-item-action>
            <v-icon>{{ routeIcon('addJob') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Add job</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item 
          :to="{ name: 'jobs'}"
          exact
        >
          <v-list-item-action>
            <v-icon>{{ routeIcon('jobs') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>All jobs</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-divider class="mb-1" />

<!--
        <v-list-group
          :value="true"
          prepend-icon="mdi-briefcase-outline"
        >
          <template v-slot:activator>
            <v-list-item-title>Jobs</v-list-item-title>
          </template>

          <v-list-item
            v-if="$store.getters.can('ADD_NEW_JOB')"
            :to="{ name: 'addJob'}"
          >
            <v-list-item-action>
              <v-icon>{{ routeIcon('addJob') }}</v-icon>
            </v-list-item-action>
            <v-list-item-content>
              <v-list-item-title>Add job</v-list-item-title>
            </v-list-item-content>
          </v-list-item>

          <v-list-item :to="{ name: 'jobs'}">
            <v-list-item-action>
              <v-icon>{{ routeIcon('jobs') }}</v-icon>
            </v-list-item-action>
            <v-list-item-content>
              <v-list-item-title>All jobs</v-list-item-title>
            </v-list-item-content>
          </v-list-item>

          <v-divider class="mb-1" />
        </v-list-group>

-->

        <v-list-item :to="{ name: 'hosts'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('hosts') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Hosts</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'hashes'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('hashes') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Hashes</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'dictionaries'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('dictionaries') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Dictionaries</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'pcfg'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('pcfg') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>PCFG</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'rules'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('rules') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Rules</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'charsets'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('charsets') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Charsets</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'masks'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('masks') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Masks</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <!--<v-list-item :to="{ name: 'files'}">-->
        <!--<v-list-item-action>-->
        <!--<v-icon>{{ routeIcon('') }}</v-icon>-->
        <!--</v-list-item-action>-->
        <!--<v-list-item-content>-->
        <!--<v-list-item-title>Encrypted files</v-list-item-title>-->
        <!--</v-list-item-content>-->
        <!--</v-list-item>-->

        <v-list-item :to="{ name: 'markovChains'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('markovChains') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Markov chains</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <!--<v-list-item :to="{ name: 'server'}">-->
        <!--<v-list-item-action>-->
        <!--<v-icon>{{ routeIcon('') }}</v-icon>-->
        <!--</v-list-item-action>-->
        <!--<v-list-item-content>-->
        <!--<v-list-item-title>Control</v-list-item-title>-->
        <!--</v-list-item-content>-->
        <!--</v-list-item>-->

        <v-divider class="mb-1" />

        <v-list-item
          v-if="$store.getters.can('MANAGE_USERS')"
          :to="{ name: 'manageUsers'}"
        >
          <v-list-item-action>
            <v-icon>{{ routeIcon('manageUsers') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Manage users</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'myAccount'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('myAccount') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>My account</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'settings'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('settings') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Settings</v-list-item-title>
          </v-list-item-content>
        </v-list-item>

        <v-list-item :to="{ name: 'server'}">
          <v-list-item-action>
            <v-icon>{{ routeIcon('server') }}</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>Server monitor</v-list-item-title>
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
        <!--<v-list-item :to="{ name: 'manageUsers'}" v-if="$store.getters.can('MANAGE_USERS')">-->
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
      <v-toolbar-title>{{ $store.state.project }}</v-toolbar-title>
      <!--
      <router-link :to="{ name: 'home'}" class="textLogo">
        <span class="logoSmallText" v-text="$vuetify.theme.project"></span>
      </router-link>
      -->
      <v-spacer />      
      <v-badge
        color="red"
        overlap
        top
        left
      >
        <span
          v-if="notificationsCount > 0"
          slot="badge"
        >{{ notificationsCount }}</span>
        <v-btn
          :icon="$vuetify.breakpoint.xsOnly"
          text
          @click.stop="toggleNotifications"
        >
          <v-icon :left="$vuetify.breakpoint.smAndUp">
            {{ notificationsCount > 0 ? 'mdi-bell-ring' : 'mdi-bell-outline' }}
          </v-icon>
          <span v-show="$vuetify.breakpoint.smAndUp">Alerts</span>
        </v-btn>
      </v-badge>
      <v-btn
        :icon="$vuetify.breakpoint.xsOnly"
        text
        @click.stop="$store.dispatch('signOut')"
      >
        <v-icon :left="$vuetify.breakpoint.smAndUp">
          mdi-logout-variant
        </v-icon>
        <span v-show="$vuetify.breakpoint.smAndUp">Sign out</span>
      </v-btn>
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
      width="400"
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
  import { routeIcon } from '@/router'
  import store from '@/store'

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
    async beforeRouteEnter (to, from, next) {
      const loggedIn = await store.dispatch('resume')
      if (!loggedIn) {
        if (from.path === '/login') {
          next()
        }
        sessionStorage.setItem('loginRedirect', to.path)
        next('/login')
      } else {
        next()
      }
    },
    mounted: function () {
      /*
      if (!this.$store.user.loggedIn) {
        this.$router.push({
          name: 'login'
        })
      }
      */
      this.getNotificationsCount();
      this.interval = setInterval(function () {
        this.getNotificationsCount()
      }.bind(this), 10000)
    },
    beforeDestroy: function () {
      clearInterval(this.interval)
    },
    methods: {
      routeIcon,
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
