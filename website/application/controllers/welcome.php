<?php

class Welcome extends Controller {

	function Welcome()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$this->load->view('front/indexa');
	}
}
?>