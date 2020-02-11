webpackJsonp([24],{463:function(e,t,a){"use strict";function n(e){a(698)}Object.defineProperty(t,"__esModule",{value:!0});var i=a(556),s=a(700),o=a(32),r=n,l=o(i.a,s.a,!1,r,"data-v-85a0a08e",null);t.default=l.exports},556:function(e,t,a){"use strict";t.a={name:"HostsView",data:function(){return{interval:null,status:"active",search:"",viewHidden:!1,totalItems:0,pagination:{},loading:!0,headers:[{text:"Name",align:"start",value:"domain_name"},{text:"IP address",value:"ip_address",align:"end",sortable:!1},{text:"Operating system",value:"os_name",align:"end",sortable:!1},{text:"Processor",value:"p_model",align:"end",width:"200",sortable:!1},{text:"Active jobs",value:"jobs",align:"center",sortable:!1},{text:"Online",value:"last_active",align:"center",sortable:!1},{text:"Show or hide",value:"deleted",sortable:!1,align:"center"}],hosts_statuses:[],hosts:[]}},watch:{pagination:{handler:"updateList",deep:!0},"$route.name":"updateList",viewHidden:"updateList"},mounted:function(){this.interval=setInterval(function(){this.loadHosts()}.bind(this),5e3)},beforeDestroy:function(){clearInterval(this.interval)},methods:{parseTimeDelta:function(e){return null!==e&&void 0!==e?this.$moment.utc(e).fromNow():"Unknown"},loadHosts:function(){var e=this;this.axios.get(this.$serverAddr+"/hosts",{params:{page:this.pagination.page,per_page:this.pagination.itemsPerPage,order_by:this.pagination.sortBy[0],descending:this.pagination.sortDesc[0],name:this.search,showDeleted:this.viewHidden}}).then(function(t){e.hosts=t.data.items,e.totalItems=t.data.total,e.loading=!1})},updateList:function(){this.loading=!0,this.loadHosts()},hideJob:function(e){var t=this;this.loading=!0,this.axios.delete(this.$serverAddr+"/hosts/"+e).then(function(e){t.loadHosts()})}}}},698:function(e,t,a){var n=a(699);"string"==typeof n&&(n=[[e.i,n,""]]),n.locals&&(e.exports=n.locals);a(455)("022b18c0",n,!0,{})},699:function(e,t,a){t=e.exports=a(454)(!0),t.push([e.i,".middle[data-v-85a0a08e]{vertical-align:middle}.cont[data-v-85a0a08e]{height:100%}.oneline[data-v-85a0a08e]{white-space:nowrap;overflow:hidden;text-overflow:ellipsis;display:block;width:200px;line-height:50px;height:50px}.inheritColor[data-v-85a0a08e]{color:inherit!important}","",{version:3,sources:["/home/runner/work/fitcrack/fitcrack/webadmin/fitcrackFE/src/components/host/hostsView.vue"],names:[],mappings:"AACA,yBACE,qBAAuB,CACxB,AACD,uBACE,WAAa,CACd,AACD,0BACE,mBAAoB,AACpB,gBAAiB,AACjB,uBAAwB,AACxB,cAAe,AACf,YAAa,AACb,iBAAkB,AAClB,WAAa,CACd,AACD,+BACE,uBAA0B,CAC3B",file:"hostsView.vue",sourcesContent:["\n.middle[data-v-85a0a08e] {\n  vertical-align: middle;\n}\n.cont[data-v-85a0a08e] {\n  height: 100%;\n}\n.oneline[data-v-85a0a08e] {\n  white-space: nowrap;\n  overflow: hidden;\n  text-overflow: ellipsis;\n  display: block;\n  width: 200px;\n  line-height: 50px;\n  height: 50px;\n}\n.inheritColor[data-v-85a0a08e] {\n  color: inherit !important;\n}\n\n"],sourceRoot:""}])},700:function(e,t,a){"use strict";var n=function(){var e=this,t=e.$createElement,a=e._self._c||t;return a("div",{staticClass:"cont"},[a("v-text-field",{staticClass:"px-2 pt-3",attrs:{clearable:"",outlined:"","prepend-inner-icon":"mdi-table-search",label:"Search","single-line":"","hide-details":""},model:{value:e.search,callback:function(t){e.search=t},expression:"search"}}),e._v(" "),a("div",{staticClass:"d-flex justify-space-between align-center px-4 pt-2"},[a("v-switch",{staticClass:"mr-4",attrs:{label:"View hidden hosts","prepend-icon":e.viewHidden?"mdi-eye-off":"mdi-eye"},model:{value:e.viewHidden,callback:function(t){e.viewHidden=t},expression:"viewHidden"}}),e._v(" "),a("v-select",{attrs:{items:e.hosts_statuses,label:"Online status","single-line":"","item-text":"text","item-value":"text","prepend-icon":"mdi-power",clearable:""},on:{change:e.updateList},model:{value:e.status,callback:function(t){e.status=t},expression:"status"}})],1),e._v(" "),a("v-divider"),e._v(" "),a("v-data-table",{ref:"table",attrs:{headers:e.headers,items:e.hosts,search:e.search,options:e.pagination,"server-items-length":e.totalItems,loading:e.loading,"footer-props":{itemsPerPageOptions:[25,50,100],itemsPerPageText:"Hosts per page"},"fixed-header":""},on:{"update:options":function(t){e.pagination=t}},scopedSlots:e._u([{key:"item.domain_name",fn:function(t){var n=t.item;return[a("router-link",{staticClass:"middle",attrs:{to:{name:"hostDetail",params:{id:n.id}}}},[e._v("\n        "+e._s(n.domain_name+" ("+n.user.name+")")+"\n      ")])]}},{key:"item.jobs",fn:function(t){var a=t.item;return[e._v("\n      "+e._s(a.jobs.map(function(e){return 10===e.status?1:0}).reduce(function(e,t){return e+t},0))+"\n    ")]}},{key:"item.p_model",fn:function(t){var n=t.item;return[a("span",{staticClass:"oneline"},[e._v(e._s(n.p_model.replace(/(?:\(R\)|\(TM\)|Intel|AMD)/g,"")))])]}},{key:"item.last_active",fn:function(t){var n=t.item;return[n.last_active.seconds_delta>61?a("span",[e._v(e._s(e.parseTimeDelta(n.last_active.last_seen)))]):a("v-icon",{attrs:{color:"success"}},[e._v("\n        mdi-power\n      ")])]}},{key:"item.deleted",fn:function(t){var n=t.item;return[a("v-tooltip",{attrs:{top:""},scopedSlots:e._u([{key:"activator",fn:function(t){var i=t.on;return[a("v-btn",e._g({staticClass:"mx-0",attrs:{icon:""},on:{click:function(t){return e.hideJob(n.id)}}},i),[a("v-icon",[e._v("\n              "+e._s(n.deleted?"mdi-eye":"mdi-eye-off")+"\n            ")])],1)]}}],null,!0)},[e._v(" "),a("span",[e._v(e._s(n.deleted?"Show":"Hide"))])])]}}])})],1)},i=[],s={render:n,staticRenderFns:i};t.a=s}});
//# sourceMappingURL=24.2133a8ff470dbe82a5bd.js.map