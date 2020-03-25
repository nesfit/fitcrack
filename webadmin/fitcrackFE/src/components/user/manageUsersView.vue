<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <div class="users mx-auto mb-5">
      <h2>Users</h2>
      <v-card>
        <v-list two-line>
          <template v-for="(item, index) in users">
            <v-list-item :key="index">
              <v-list-item-content>
                <v-list-item-title>
                  {{ item.username }}
                </v-list-item-title>
                <v-list-item-subtitle>{{ item.mail }}</v-list-item-subtitle>
              </v-list-item-content>
              <v-list-item-action class="width150">
                <v-select
                  class="tinyselect"
                  outlined
                  hide-details
                  dense
                  item-text="name"
                  item-value="id"
                  :items="userRoles"
                  label="Role"
                  :value="item.role.id"
                  @change="updateUserRole($event, item.id)"
                />
              </v-list-item-action>
              <v-list-item-action>
                <div class="addBtnCont">
                  <v-btn
                    color="primary"
                    class="addBtn"
                    text
                    @click="editDialog=true"
                  >
                    Edit
                  </v-btn>
                </div>
              </v-list-item-action>
              <v-list-item-action>
                <v-tooltip top>
                  <template v-slot:activator="{ on }">
                    <v-btn
                      icon
                      class="mx-0"
                      @click="deleteUser(item.id)"
                      v-on="on"
                    >
                      <v-icon color="error">
                        close
                      </v-icon>
                    </v-btn>
                  </template>
                  <span>Delete user</span>
                </v-tooltip>
              </v-list-item-action>
            </v-list-item>
            <v-divider
              v-if="index + 1 < users.length"
              :key="index + 'd'"
            />
          </template>
        </v-list>
      </v-card>
      <div class="addBtnCont">
        <v-btn
          color="primary"
          class="addBtn"
          text
          @click="userDialog=true"
        >
          Add user
        </v-btn>
      </div>
    </div>
    <h2>User roles</h2>
    <div class="roleTable">
      <v-data-table
        class="elevation-1 roleTable"
        :headers="headers"
        :items="userRoles"
        :loading="loading"
        hide-default-footer
      >
        <template v-slot:item.ADD_NEW_JOB="{ item }">
          <v-checkbox
            v-model="item.ADD_NEW_JOB"
            :disabled="item.id == 1"
            @change="roleChange($event, item.id, 'ADD_NEW_JOB')"
          />
        </template>
        <template v-slot:item.MANAGE_USERS="{ item }">
          <v-checkbox
            v-model="item.MANAGE_USERS"
            :disabled="item.id == 1"
            @change="roleChange($event, item.id, 'MANAGE_USERS')"
          />
        </template>
        <template v-slot:item.id="{ item }">
          <v-tooltip top>
            <template v-slot:activator="{ on }">
              <v-btn
                v-show="item.id > 1"
                icon
                class="mx-0"
                @click="deleteRole(item.id)"
                v-on="on"
              >
                <v-icon color="error">
                  close
                </v-icon>
              </v-btn>
            </template>
            <span>Delete role</span>
          </v-tooltip>
        </template>
      </v-data-table>
      <div class="addBtnCont">
        <v-btn
          color="primary"
          class="addBtn"
          text
          @click="roleDialog=true"
        >
          Add role
        </v-btn>
      </div>
    </div>

    <v-dialog
      v-model="roleDialog"
      max-width="400px"
    >
      <v-card>
        <v-card-title>
          <h2>Add new user role</h2>
        </v-card-title>
        <v-card-text>
          <v-text-field
            v-model="newRoleName"
            label="Role name"
          />
        </v-card-text>
        <v-card-actions>
          <v-spacer />
          <v-btn
            color="primary"
            text
            @click.stop="roleDialog=false"
          >
            Cancel
          </v-btn>
          <v-btn
            color="primary"
            text
            :disabled="newRoleName==''"
            @click.stop="addRole"
          >
            Add
          </v-btn>
        </v-card-actions>
        <div
          v-if="addingRole"
          class="loadingOver"
        >
          <v-progress-circular
            size="50"
            :width="3"
            class="progress"
            indeterminate
            color="primary"
          />
        </div>
      </v-card>
    </v-dialog>

    <v-dialog
      v-model="editDialog"
      max-width="500px"
    >
      <v-card>
        <v-card-title>
          <h2>Edit user</h2>
        </v-card-title>
        <v-card-text>
          <v-form
            ref="form"
            lazy-validation
          >
            <v-text-field
              v-model="newUsername"
              label="Username"
              :counter="10"
              required
            />
            <v-text-field
              v-model="newEmail"
              label="E-mail"
              :rules="emailRules"
              required
            />
            <v-text-field
              v-model="oldPassword"
              type="password"
              label="Old password"
              required
            />
            <v-text-field
              v-model="newPassword0"
              type="password"
              label="New password"
              required
            />
            <v-text-field
              v-model="newPassword1"
              type="password"
              label="New password"
              :rules="newPasswordRules"
              required
            />
            <v-select
              v-model="newUserRoleID"
              label="User role"
              item-text="name"
              item-value="id"
              :items="userRoles"
              :rules="[v => !!v || 'Item is required']"
              required
            />
          </v-form>
        </v-card-text>
        <v-card-actions>
          <v-spacer />
          <v-btn
            color="primary"
            text
            @click.stop="editDialog=false"
          >
            Cancel
          </v-btn>
          <v-btn
            color="primary"
            text
            disabled="true"
            @click.stop="editUser"
          >
            Edit
          </v-btn>
        </v-card-actions>
        <div
          v-if="editingUser"
          class="loadingOver"
        >
          <v-progress-circular
            size="50"
            :width="3"
            class="progress"
            indeterminate
            color="primary"
          />
        </div>
      </v-card>
    </v-dialog>

    <v-dialog
      v-model="userDialog"
      max-width="500px"
    >
      <v-card>
        <v-card-title>
          <h2>Add new user</h2>
        </v-card-title>
        <v-card-text>
          <v-form
            ref="form"
            v-model="validNewUserForm"
            lazy-validation
          >
            <v-text-field
              v-model="newUsername"
              label="Username"
              :counter="10"
              required
            />
            <v-text-field
              v-model="newEmail"
              label="E-mail"
              :rules="emailRules"
              required
            />
            <v-text-field
              v-model="newPassword"
              type="password"
              label="Password"
              required
            />
            <v-select
              v-model="newUserRoleID"
              label="User role"
              item-text="name"
              item-value="id"
              :items="userRoles"
              :rules="[v => !!v || 'Item is required']"
              required
            />
          </v-form>
        </v-card-text>
        <v-card-actions>
          <v-spacer />
          <v-btn
            color="primary"
            text
            @click.stop="userDialog=false"
          >
            Cancel
          </v-btn>
          <v-btn
            color="primary"
            text
            :disabled="!validNewUserForm || newUsername == '' || newEmail == '' || newPassword == '' || newUserRoleID == ''"
            @click.stop="addUser"
          >
            Add user
          </v-btn>
        </v-card-actions>
        <div
          v-if="addingUser"
          class="loadingOver"
        >
          <v-progress-circular
            size="50"
            :width="3"
            class="progress"
            indeterminate
            color="primary"
          />
        </div>
      </v-card>
    </v-dialog>
  </div>
</template>

<script>
  export default {
    name: 'UsersView',
    data: function () {
      return {
        newRoleName: null,
        addingRole: false,
        roleDialog: false,
        newPassword: null,
        newUsername: null,
        newUserRoleID: null,
        newEmail: null,
        editingUser: false,
        editDialog: false,
        addingUser: false,
        userDialog: false,
        oldPassword: null,
        newPassword0: null,
        newPassword1: null,
        users: [],
        totalItems: 0,
        pagination: {},
        loading: true,
        headers: [
          {
            text: 'Name',
            align: 'start',
            value: 'name'
          },
          {text: 'Add new job', value: 'ADD_NEW_JOB', align: 'start'},
          {text: 'Manage users', value: 'MANAGE_USERS', align: 'start'},
          {text: '', value: 'id', align: 'end'}
        ],
        userRoles: [],
        validEditUserForm: true,
        validNewUserForm: true,
        emailRules: [
          v => !!v || 'E-mail is required',
          v => /^\w+([.-]?\w+)*@\w+([.-]?\w+)*(\.\w{2,3})+$/.test(v) || 'E-mail must be valid'
        ],
        newPasswordRules: [
          v => !!v || 'Repeat new password',
          v => this.newPassword0 != this.newPassword1 || 'match!'
        ]
      }
    },
    created: function () {
      this.loadUsers();
      this.loadRoles()
    },
    methods: {
      loadUsers() {
        this.axios.get(this.$serverAddr + '/user/').then((response) => {
          this.users = response.data.items
        })
      },
      loadRoles() {
        this.loading = true;
        this.axios.get(this.$serverAddr + '/user/role').then((response) => {
          this.userRoles = response.data.items;
          this.loading = false
        })
      },
      updateUserRole(roleId, userId) {
        this.axios.post(this.$serverAddr + '/user/role', {
          user_id: userId,
          role_id: roleId
        }).then((response) => {
          console.log(response.data);
          console.log('user role changed')
        })
      },
      roleChange(value, roleId, key) {
        this.axios.post(this.$serverAddr + '/user/role/' + roleId, {
          key: key,
          value: value
        }).then((response) => {
          this.$store.dispatch('refreshUser')
          console.log(response.data);
          console.log('user role changed')
        })
      },
      addRole() {
        this.addingRole = true;
        this.axios.post(this.$serverAddr + '/user/role/new', {
          name: this.newRoleName
        }).then((response) => {
          console.log(response.data);
          this.loadRoles();
          this.roleDialog = false;
          this.newRoleName = '';
          this.addingRole = false
        })
      },
      deleteRole(id) {
        this.axios.delete(this.$serverAddr + '/user/role/' + id).then((response) => {
          console.log(response.data);
          this.loadRoles()
        })
      },
      addUser() {
        this.addingUser = true;
        this.axios.post(this.$serverAddr + '/user/', {
          username: this.newUsername,
          mail: this.newEmail,
          password: this.newPassword,
          role_id: this.newUserRoleID
        }).then((response) => {
          console.log(response.data);
          this.loadUsers();
          this.userDialog = false;
          this.addingUser = false;
          this.newUsername = '';
          this.newEmail = '';
          this.newPassword = '';
          this.newUserRoleID = ''
        })
      },
      editUser() {
        this.editingUser = true;
        this.axios.post(this.$serverAddr + '/user/', {
          username: this.newUsername,
          mail: this.newEmail,
          password: this.newPassword,
          role_id: this.role_id //insert anything with the same ID => rewrite row
        }).then((response) => {
          console.log(response.data);
          this.loadUsers();
          this.editDialog = false;
          this.edittingUser = false;
          this.newUsername = '';
          this.newEmail = '';
          this.newPassword = '';
          this.newUserRoleID = ''
        })
      },
      deleteUser(id) {
        this.axios.delete(this.$serverAddr + '/user/' + id).then((response) => {
          console.log(response.data);
          this.loadUsers()
        })
      },
    }
  }
</script>

<style scoped>
  .tinyselect {
    max-width: 20ch;
  }

  .users {
    margin-top: 2em;
    max-width: 550px;
  }

  h2 {
    padding-bottom: 5px;
    text-align: center;
  }

  .roleTable {
    max-width: 550px;
    margin: auto;
  }

  .width150 {
    min-width: 100px;
  }

  .addBtnCont {
    text-align: right;
  }

  .addBtn {
    display: inline-block;
    margin-right: 0;
  }

  .loadingOver {
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;

  }

  .loadingOver .progress {
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    margin: auto;
    position: absolute;
  }
</style>
