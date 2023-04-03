boinccmd --project 127.0.0.1/fitcrack/ detach || true
KEY=$(boinccmd --create_account 127.0.0.1/fitcrack/ xwagne10@vutbr.cz fitcrack dukek | grep -oP 'account key: \K\w+')
boinccmd --project_attach 127.0.0.1/fitcrack/ $KEY