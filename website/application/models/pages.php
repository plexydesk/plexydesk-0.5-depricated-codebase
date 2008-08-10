<?php
	class Pages extends Model 
	{
		function Pages()
	    {
	        parent::Model();
	    }
		// home page
		function home_page()
		{
			$this->load->view('front/home_body');	
		}
		function home_side_bar()
		{
			// preparing the latest blog entries auery
			$this->db->select('entry_id, entry_title, entry_intro, entry_added');
			$this->db->where('entry_status', 1);
			$this->db->limit(3);
			$this->db->order_by("entry_added", "desc");
			
			$entries['latest_entries'] = $this->db->get('tbl_blog_entries');
			
			$this->load->view('front/home_side_bar', $entries);
		}
		//plexy desk page
		function plexydesk()
		{
			$this->load->view('front/plexydesk');
		}
		//plexy desk download page
		function download()
		{
			$this->load->view('front/download');
		}
		//plexy desk screenshots page
		function screenshots()
		{
			$this->load->view('front/screenshots');
		}
		//plexy desk help us page
		function help_us()
		{
			$this->load->view('front/help_us');
		}
		//plexy desk help page
		function help()
		{
			$this->load->view('front/help');
		}
		//plexy desk feedback page
		function feedback()
		{
			$this->load->view('front/feedback');
		}
		//plexy desk sponcer page
		function sponcer()
		{
			$this->load->view('front/sponcer');
		}
		//plexy desk hacking page
		function hacking()
		{
			$this->load->view('front/hacking');
		}
		//plexy desk ideas page
		function ideas()
		{
			$this->load->view('front/ideas');
		}
		//plexy desk join page
		function join()
		{
			$this->load->view('front/join');
		}
	}
?>