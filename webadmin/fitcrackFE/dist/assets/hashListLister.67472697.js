import{n as r}from"./index.da48d82d.js";import"./vendor.6aa46313.js";var o=function(){var t=this,a=t.$createElement,e=t._self._c||a;return e("div",{staticClass:"cont"},[e("v-toolbar",{attrs:{flat:""}},[e("h2",[t._v("Hashlists")]),e("v-spacer"),e("v-btn",{attrs:{text:"",color:"primary",to:{name:"createHashlist"}}},[e("v-icon",{attrs:{left:""}},[t._v(" mdi-book-plus ")]),t._v(" Create new ")],1)],1),e("v-divider"),e("v-data-table",{ref:"table",attrs:{headers:t.headers,items:t.hashlists,options:t.pagination,"server-items-length":t.totalItems,loading:t.loading,"footer-props":{itemsPerPageOptions:[25,50,100],itemsPerPageText:"Hashlists per page"}},on:{"update:options":function(s){t.pagination=s}},scopedSlots:t._u([{key:"item.name",fn:function(s){var i=s.item;return[e("router-link",{attrs:{to:{name:"hashlistDetail",params:{id:i.id}}}},[t._v(" "+t._s(i.name)+" ")])]}}])})],1)},l=[];const h={name:"HashListLister",data:function(){return{interval:null,status:"active",totalItems:0,pagination:{},loading:!0,headers:[{text:"Name",value:"name",align:"start",sortable:!1},{text:"Hash type",value:"hash_type_name",align:"end",sortable:!0},{text:"Entries",value:"hash_count",align:"end",sortable:!0}],hashlists:[]}},watch:{pagination:{handler(){this.loading=!0,this.loadHashes()},deep:!0}},mounted(){this.interval=setInterval(function(){this.loadHashes()}.bind(this),5e3)},beforeDestroy:function(){clearInterval(this.interval)},methods:{loadHashes(){this.axios.get(this.$serverAddr+"/hashlist",{params:{page:this.pagination.page,per_page:this.pagination.rowsPerPage,order_by:this.pagination.sortBy,descending:this.pagination.descending}}).then(t=>{this.hashlists=t.data.items,this.totalItems=t.data.total,this.loading=!1})},updateStatus:function(t){this.status=t,this.$refs.table.updatePagination({page:1,totalItems:this.totalItems})}}},n={};var d=r(h,o,l,!1,u,"0597d52c",null,null);function u(t){for(let a in n)this[a]=n[a]}var _=function(){return d.exports}();export{_ as default};
