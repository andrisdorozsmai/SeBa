//
// white_dwarf.C
//

#include "white_dwarf.h"
#include "hertzsprung_gap.h"
#include "sub_giant.h"
#include "super_giant.h"
#include "helium_star.h"
#include "helium_giant.h"

white_dwarf::white_dwarf(super_giant & g, stellar_type wd_type) : single_star(g) {
      delete &g;
      white_dwarf_type = wd_type;
    
      real m_tot    = get_total_mass();
//    (GN+SilT Mar 2010) was cnsts.parameters(kanonical_neutron_star_mass)
      core_mass     = min(0.999999*cnsts.parameters(Chandrasekar_mass),
			  core_mass); 

      envelope_mass = m_tot - core_mass;
      accreted_mass = 0;

      lose_envelope_decent();

// (GN+SPZ May  4 1999) last update age is time of previous type change
//      last_update_age = next_update_age;
    last_update_age = 0.;
    relative_age = 0.;//1 + nucleair_evolution_time();

      instantaneous_element();
      update();

      post_constructor();
}

white_dwarf::white_dwarf(sub_giant & s, stellar_type wd_type) : single_star(s) {
      delete &s;
      white_dwarf_type = wd_type;

      real m_tot    = get_total_mass();
      core_mass     = min(0.999999*cnsts.parameters(Chandrasekar_mass),
			  core_mass); 
      envelope_mass = m_tot - core_mass;
      accreted_mass = 0;

// (GN+SPZ May  4 1999) last update age is time of previous type change
//      last_update_age = next_update_age;
    last_update_age = 0.;

      lose_envelope_decent();

    relative_age = 0.;//1 + nucleair_evolution_time();

      instantaneous_element();
      update();

      post_constructor();

}

white_dwarf::white_dwarf(hertzsprung_gap & s, stellar_type wd_type) : single_star(s) {
      delete &s;
      white_dwarf_type = wd_type;

      real m_tot    = get_total_mass();
      core_mass     = min(0.999999*cnsts.parameters(Chandrasekar_mass),
			  core_mass); 
      envelope_mass = m_tot - core_mass;
      accreted_mass = 0;

// (GN+SPZ May  4 1999) last update age is time of previous type change
//      last_update_age = next_update_age;
    last_update_age = 0.;

      lose_envelope_decent();

    relative_age = 0.;//1 + nucleair_evolution_time();

      instantaneous_element();
      update();

      post_constructor();

}


white_dwarf::white_dwarf(helium_star & h, stellar_type wd_type) : single_star(h) {
 
        delete &h;
    white_dwarf_type = wd_type;

	real m_tot    = get_total_mass();
	core_mass     = min(0.999999*cnsts.parameters(Chandrasekar_mass),
			    core_mass); 
	envelope_mass = m_tot - core_mass;
	accreted_mass = 0;

// (GN+SPZ May  4 1999) last update age is time of previous type change
//        last_update_age = next_update_age;
    last_update_age = 0.;

	lose_envelope_decent();
	
    relative_age = 0.;//1 + nucleair_evolution_time();

	instantaneous_element();
	update();

	post_constructor();

}


white_dwarf::white_dwarf(helium_giant & h, stellar_type wd_type) :  single_star(h) {

        delete &h;
    white_dwarf_type = wd_type;

	real m_tot    = get_total_mass();
	core_mass     = min(0.999999*cnsts.parameters(Chandrasekar_mass),
			    core_mass); 
	envelope_mass = m_tot - core_mass;
	accreted_mass = 0;

// (GN+SPZ May  4 1999) last update age is time of previous type change
//        last_update_age = next_update_age;
    last_update_age = 0.;

	lose_envelope_decent();

    relative_age = 0.;//1 + nucleair_evolution_time();

	instantaneous_element();
	update();

	post_constructor();

}
      
void white_dwarf::instantaneous_element() {
//cerr << "white_dwarf::instantaneous_element"<<endl;

        next_update_age = relative_age + cnsts.safety(maximum_timestep);

	luminosity = 40;
	// m_rel may not become larger than M_Ch
//	real m_rel = min(0.999999, core_mass/cnsts.parameters(Chandrasekar_mass));

	// Nauenberg, M, 1972, Apj 175, 417
	// mu=4.039 is the mean molecular weight.
	real mu = 2.;

	effective_radius =
	core_radius      =
	radius           = white_dwarf_radius(core_mass, relative_age);
	  //               min(0.1, (0.0225/mu)*
	  //               sqrt(1./pow(m_rel, cnsts.mathematics(two_third))
	  //	           - pow(m_rel, cnsts.mathematics(two_third))));
}       


void white_dwarf::evolve_element(const real end_time) {

         real dt = end_time - current_time;
        current_time = end_time;
        relative_age += dt;

// done in add_mass_to_accretor
//        accrete_from_envelope(dt);

        if (core_mass > cnsts.parameters(Chandrasekar_mass) ||
	    core_mass <= cnsts.safety(minimum_mass_step)) {
            // || accreted_mass > 0.2) {
	    // (GN Mar 26 1999) test edge lid detanation 
	    // needs more research

	  if (is_binary_component()) 
	    get_binary()->dump("binev.data", false);
	  else
	    dump("binev.data", false);
	  
	  star_transformation_story(Disintegrated);
	  new disintegrated(*this);
	  // new neutron_star(*this);    // AIC
	  return;
        }

//    real t_nuc = nucleair_evolution_time();
        next_update_age = relative_age + cnsts.safety(maximum_timestep);

    if (relative_age <= 0.){
  //      if (t_nuc>=relative_age) {
           luminosity = 40;
        //relative_age = t_nuc;
        relative_age = 0.;
        
        }
        else {
	  // (GN May  4 1999) fit to Driebe et al 1999
	  real fit_mass = min(0.6, max(0.18, get_total_mass()));
	  real l_max = pow(10, (3.83 - 4.77* fit_mass));
	  luminosity = l_max/pow((relative_age), 1.4);
	}

	
//	real m_rel = min(0.999999, core_mass/cnsts.parameters(Chandrasekar_mass));

	   // Nauenberg, M, 1972, Apj 175, 417
	   // mu=4.039 is the mean molecular weight.
           real mu = 2.;
	   core_radius =
	     radius           = white_dwarf_radius(core_mass, relative_age);

//             radius = min(0.1, (0.0225/mu)
//	            * sqrt(1./pow(m_rel, cnsts.mathematics(two_third))
//                    - pow(m_rel, cnsts.mathematics(two_third))));

	// (GN+SPZ May  3 1999) critical mass for nova (Livio 1993; Saas-Fee)
	//   real m_crit = 1.7e-4*pow(get_total_mass(),-0.7)
	//               * pow(69.9*radius,2.8); // radius in 10^9 cm

	//if (envelope_mass >= m_crit) 
	//   thermo_nucleair_flash(dt);


//	   if (envelope_mass >= core_mass
//	                     * cnsts.parameters(thermo_nuclear_flash))
//	     thermo_nucleair_flash(dt);
       
	   update();
}

void white_dwarf::update() {

        detect_spectral_features();
// (GN+SPZ May  4 1999) last_update_age now used as time of last type change
//  last_update_age = relative_age;
        effective_radius = max(effective_radius, radius);

}


star* white_dwarf::subtrac_mass_from_donor(const real dt, real& mdot) {
    
    mdot = mdot_limit(dt, mdot);
    
        if (mdot<=envelope_mass)
           envelope_mass -= mdot;
        else {
	  mdot -= envelope_mass;
	  envelope_mass = 0;
	  if (mdot >= core_mass)
	    mdot = core_mass - cnsts.safety(minimum_mass_step);
	  core_mass -= mdot;
	}

        return this;
}

real white_dwarf::add_mass_to_accretor(real mdot, bool hydrogen, const real dt) {

        if (mdot<0) {
           cerr << "white_dwarf::add_mass_to_accretor(mdot="
                 << mdot << ")"<<endl;
           cerr << "mdot (" << mdot << ") smaller than zero!" << endl;

	   mdot = 0;
        }

    //For white dwarfs no difference currently between hydrogen/helium/.. accretion
	
// (GN+SPZ May  3 1999)
	real mu =1;
	if (is_binary_component() && 
	    !get_companion()->hydrogen_envelope_star()) mu =2;
	

	if (mdot >= eddington_limit(radius, dt, mu)) {
	  effective_radius = min(10., get_binary()->roche_radius(this));
	}

        mdot = accretion_limit(mdot, dt, hydrogen);

	// (Madelon+GN May 16 2011)
	// Implement accretion to core via retention efficiencies
	// Everything is dealt with in ::accretion_limit so
	// just add mdot to core

	  core_mass += mdot;
	  accreted_mass += mdot;

        adjust_accretor_age(mdot);
        //envelope_mass += mdot;
	relative_mass = max(relative_mass, get_total_mass());

	set_spec_type(Accreting);

        return mdot;

     }


real  white_dwarf::accretion_limit(const real mdot, const real dt) {
    //needed for double_star::zeta and double_star::perform_mass_transfer
    return accretion_limit(mdot, dt, true);
}


real  white_dwarf::accretion_limit(const real mdot, const real dt, bool hydrogen) {

  if (dt < 0) return mdot;

  // (GN+Madelon May 12 2011) implement retention efficiencies
  //return min(maximum_steady_burning(dt), mdot);  

  real dmdt = 1.e-6*mdot/dt;
  real eta = retention_efficiency(dmdt, get_total_mass(), hydrogen);
  
  return mdot*eta;
}


real white_dwarf::retention_efficiency(real dmdt, real M_WD, bool hydrogen) {

  real eta = 1.;

  if (hydrogen) {

    real eta_H = retention_H(dmdt, M_WD);
    real eta_He = retention_He(eta_H*dmdt, M_WD);
    eta = eta_H*eta_He;
    
  } else {

    eta = retention_He(dmdt, M_WD);
  }

  return eta;
}


// (Madelon+GN May 16 2011)
// Retention efficiencies according to Nomoto
// (SilT Jun 19th 2012) rewritten equations
// (SilT Oct 16th 2013) no wind stripping -> c1 = 0 
real white_dwarf::retention_H(real dmdt, real M_WD) {

    if (dmdt < 1.e-7 || M_WD < 0.6) return 0.;
 
    real logdmdt = log10(dmdt); 
    real eta = 1.;
    
    real M_cr_log = log10(7.5e-7 * (M_WD - 0.4));
    real M_st = 3.1e-7 * (M_WD - 0.54);
    real M_st_log = log10(M_st);
    
    if (logdmdt > M_cr_log) 
        // (SilT Oct 16th 2013) no wind stripping -> c1 = 0 
        eta = pow(10, M_cr_log) / dmdt;   
//        eta = 4. * pow(10,M_cr_log) / (3.*pow(10,M_cr_log) + dmdt);  
    if (logdmdt < M_st_log)
        eta = (logdmdt - 1.e-7) / (M_st_log - 1.e-7);
      
    return eta;
}


real white_dwarf::retention_He(real dmdt, real M_WD) {
  
  real logdmdt = log10(dmdt);
  real eta = 0.;

  if (logdmdt > -7.8 && logdmdt < -5.9) 
    eta = -0.175 * pow(logdmdt+5.35,2) + 1.05;
  if (logdmdt > -5.9 && logdmdt < -5.0) eta =  1.;

  return eta;
}



void white_dwarf::adjust_accretor_age(const real mdot,
				      const bool rejuvenate) {
//        real m_rel_new;
//        real m_tot_new = get_total_mass() + mdot;
//        if (m_tot_new>relative_mass)
//           m_rel_new = m_tot_new;
//        else m_rel_new = relative_mass;

    real t_nuc_old = 0.;//nucleair_evolution_time();
//	real z_new = get_metalicity();
    real t_nuc_new = 0.;//nucleair_evolution_time(m_rel_new, m_tot_new, z_new);

        real dtime = relative_age - t_nuc_old;

        relative_age = t_nuc_new + dtime
	             * (1-pow(mdot/(get_total_mass()+mdot),
		     	      cnsts.parameters(rejuvenation_exponent)));
	
     }

real white_dwarf::zeta_thermal() {

     // Based on white dwarf mass radius relation.
     // zeta = (m/r)dr/dm;
  
     return -cnsts.mathematics(one_third);
}

real white_dwarf::zeta_adiabatic() {

     // Based on white dwarf mass radius relation.
     // zeta = (m/r)dr/dm;

     return -cnsts.mathematics(one_third);
}

star* white_dwarf::merge_elements(star* str) {

     envelope_mass = 0;		//Destroy disc.
	
     real merger_core = str->get_core_mass();

     add_mass_to_accretor(str->get_envelope_mass(), str->hydrogen_envelope_star());

     if (relative_mass<get_total_mass() + merger_core)
       relative_mass=get_total_mass() + merger_core;
     core_mass += merger_core;

     spec_type[Merger]=Merger;
     instantaneous_element();

     return this;
}

star* white_dwarf::reduce_mass(const real mdot) {

      if (envelope_mass<+mdot) {
	real mdot_rest = mdot - envelope_mass;
	envelope_mass = 0;
	if (mdot_rest >= core_mass)
	  mdot_rest = core_mass - cnsts.safety(minimum_mass_step);
	core_mass -= mdot_rest;
      }
      else envelope_mass -= mdot;

      return this;
}


real white_dwarf::gyration_radius_sq() {

  return cnsts.parameters(homogeneous_sphere_gyration_radius_sq); 
}

//stellar_type white_dwarf::get_element_type(){
//
//  if (core_mass     < cnsts.parameters(helium_dwarf_mass_limit) &&
//      relative_mass < cnsts.parameters(upper_ZAMS_mass_for_degenerate_core))
//    return Helium_Dwarf;
//  
//  else if (relative_mass >= cnsts.parameters(super_giant2neutron_star) &&
//           core_mass > cnsts.parameters(carbon_dwarf_mass_limit))
//    return Oxygen_Dwarf;
//  
//  else 
//    return Carbon_Dwarf;
//}


real white_dwarf::get_evolve_timestep() {
        
    return max(next_update_age - relative_age, 0.0001);
}
