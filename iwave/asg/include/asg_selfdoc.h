char * sdoc[] = {
" IWAVE Variable Density Acoustic Wave Equation Solver, Version 1.1",
"  ",
"  ",
" Purpose: solve the variable density acoustic wave equation in 1, 2 or",
" 3 spatial dimensions, output pressure traces at specified sample rates",
" and/or movie frames of pressure. Uses staggered grid finite difference",
" scheme of order 2 in time and 2k in space, k=1,...,7, derived from",
" pressure-velocity form of acoustodynamics. Either pressure-free",
" (reflecting) or absorbing (PML) boundary condition on each boundary",
" face of simulation hypercube.",
" ",
" Authors: Igor S. Terentyev, Tetytana Vdovina, William W. Symes,", 
" Xin Wang, Dong Sun",
" ",
" July 2010",
" ",
" Usage: ",
"   ",
"   asg.x par=[name of parameter file]",
"   ",
" Command name (asg.x) is a standin for any valid form of invocation,",
" including mpirun or mpiexec, with appropriate path information. ",
" ",
" Use Notes: [see html documentation for detailed info on many use cases]",
" ",
"   Required parameter",
"   ",
"     datafile = [string] - name of (existing) SU (SEGY) trace - headers",
"                define acquisition geometry. Required headers: ns, dt,",
"                delrt, gx, gy, gelev, sx, sy, selev",
"   ",
"   Optional parameters ",
" ",
"   [Those stated without defaults are required for this use case.]",
"     ",
"     Material parameters. ",
"     ",
"       velocity = [string] - RSF header file for sound (p-) velocity, ",
"           also defines computational grid for pressure",
"       density  = [string] or rho = [string] [Default: homogeneous, 1000 kg/m^3] - ",
"           RSF header file for material density - if this keyword omitted, ",
"           default constant value is applied throughout grid",
" ",
"     Spatial interpolation order for sampling (traces) and adjoint sampling (source),"
"     either 0 (greatest-integer sampling) or 1 (multilinear sampling)",
"       ",
"       sampord   = [int] [Default = 0].",
"     ",
"     Simple isotropic point source representation, normalized so that ",
"     solution has specified waveform at specified distance 3D in homog ",
"     fluid\n"
"     ",
"       srctype   = point",
"       ",
"       source    = [string] name of SU file from which to read target",
"       propagating pulse (only the first trace will be read).  ",
"        ",
"       refvel    = [float] [Default = 1.5 m/ms] velocity in hypothetical",
"       homogeneous fluid ",
"       ",
"       refbou    = [float] [Default = 0.001 kg/m^3] buoyancy in hypothetical",
"       homogeneous fluid",
"       ",
"       refdist   = [float] [Default = 1000 m] distance from source in",
"       hypothetical homogeneous fluid.",
" ",
"     Finite difference scheme [See Additional Notes on Acoustic Modeling, below]",
"                   ",
"       cmax = [float] [Default=5 m/ms] maximum permitted velocity",
"       ",
"       cmin = [float] [Default=0.2*cmax] minimum permitted velocity",
"       ",
"       cfl = [float] CFL number [Default=0.4]",
"       ",
"       scheme_phys, scheme_npml = [string], [string]",
"       [Defaults = 24, 24] - time and space orders for physical, PML domains.",
"       Legal values: 22, 24, 210, 2k.",
"       ",
"       k_phys, k_npml = [int], [int] - half-orders in space for (2,2k) schemes",
"       scheme. Valid range = 1 through 7.", 
"       ",
"       PML layers descriptors: construction of PML layers depends on ",
"         ",
"         fpeak = [float] [Default=0.01KHz] nominal central frequency for",
"         wavefield, used only to calibrate thickness of PML layers. As with",
"         cmin, no role in accuracy control.",
"         ",
"         npml_ampl = [float], [Default = 10.3616], - damping factor in",
"         absorbing layers",
"         ",
"         nl1,nr1 = [float], [float] [Defaults = 0.0, 0.0], for axis 1;",
"         nl2,nr2 = [float], [float] [Defaults = 0.0, 0.0], for axis 2;",
"         nl3,nr3 = [float], [float] [Defaults = 0.0, 0.0], for axis 3 - ",
"         Thicknesses of left and right absorbing layers along each axis, in",
"         wavelengths at nominal peak frequency (fpeak) and maximum velocity",
"       ",
"     ",
"     Domain decomposition via MPI [See Notes on Modeling with IWAVE, below]:",
"       ",
"        mpi_np1   = [int] [Default = 1],",
"        mpi_np2   = [int] [Default = 1],",
"        mpi_np3   = [int] [Default = 1].-",
"        Numbers of domains along each axis.    ",
  " ",
"     ",
"     Threaded parallelism via OpenMP:",
"       ",
"       omp_nt    = [int] [Default = 1] number of OpenMP threads - significant ",
"       only if OpenMP is enabled.during install",
"       ",
"     ",
"     Dump Flags - switch verbose output, written to cout[rank] in working directory ",
"     (see Output in Notes, below):",
"       ",
"       ",
"       dump_pi   = [int] [Default = 0] - dump parallel/dom decomp info",
"       ",
"       dump_lda  = [int] [Default = 0] - dump grid data for allocated arrays, including ",
"                   ghost cell zones.",
"       ",
"       dump_ldc  = [int] [Default = 0] - dump grid data for arrays updated in this process ",
"                   (not including ghost cells).",
"       ",
"       dump_lds  = [int] [Default = 0] - dump grid data for send arrays",
"       ",
"       dump_ldr  = [int] [Default = 0] - dump grid data for receive arrays",
"       ",
"       dump_term = [int] [Default = 0] - dump trace sampling data",
"       ",
"       printact  = [int] [Default = 0] - timestep action output: 0 - no output, ",
"                   1 - timestep number, 2 - detailed actions within timestep.",
"       ",
" ",
" Alternate Use Cases",
" ",
" This section describes various alternative tasks implemented by asg.x, and ",
" corresponding parameter selection patterns.",
" ",
" ",
" 1. Create/modify output data file: it is possible to take headers from one ",
" file of SEGY traces, yet write the output to another. To do this, specify ",
" the source of the trace headers as hdrfile, and the output data file as ",
" datafile (as in the typical use case):",
" ",
" hdrfile = [string]",
" datafile = [string]",
" ",
" In this case, the output file is overwritten in its entirety (including ",
" any SEGY header information).",
" ",
" ",
" ",
" 2. Alternate methods for specifying computational grid and variable bulk ",
" modulus: it is actually the bulk modulus field which the finite difference ",
" scheme uses internally. It is possible to prescribe it directly:",
" ",
" bulkmod = [string], or kappa = [string]",
" ",
" ",
" ",
" 3. Alternate method for specifying computational grid and constant material ",
" parameters:  specifies grid in fashion of RSF header, constant values for ",
" bulk modulus and buoyancy (or equivalent parameters):",
" ",
" n1 = [int] [default = 1],",
" n2 = [int] [default = 1],",
" n3 = [int] [default = 1],",
" d1 = [float] [default = 1.0],",
" d2 = [float] [default = 1.0],",
" d3 = [float] [default = 1.0],",
" o1 = [float] [default = 0.0],",
" o2 = [float] [default = 0.0],",
" o3 = [float] [default = 0.0],",
" x_axis = [int] [default = 2],",
" y_axis = [int] [default = 3],",
" z_axis = [int] [default = 1],",
" refkappa = [float] [default = 2250 MPa],",
" refbuoy  = [float] [default = 0.001 m^3/kg],",
" refvel   = [float] [default = 1.5 m/ms],",
" refden   = [float] [default = 1000 kg/m^3]",
" ",
" Notes: axis information supplied mostly for test purposes - since the material ",
" modeled is homogeneous, the axis labeling is immaterial. The values of bulk ",
" modulus (refkappa) and buoyancy (refbuoy) have priority: the other two are used",
" only if one of these is not given.",
" ",
" ",
" 4. Direct specification of buoyancy from RSF file, rather than density:",
" ",
" buoyancy = [string]",
" ",
" ",
" ",
" 5. Direct specification of buoyancy or density on shifted grids:",
" ",
"   buoyancy1 = [string] and",
"   buoyancy2 = [string] and",
"   buoyancy3 = [string], ",
"   ",
"   or",
"   ",
"   density1  = [string] and",
"   density2  = [string] and",
"   density3  = [string] ",
"   ",
"   or",
"   ",
"   rho1      = [string] and",
"   rho2      = [string] and",
"   rho3      = [string] ",
"   ",
" For this option, the RSF files should represent shifted values of the fields, by ",
" one-half grid cell along each axis. The option is provided mostly for test purposes:",
" it allows the use of directly sampled, rather than interpolated, values of buoyancy",
" in case the latter is somehow defined as an actual function. Thus interpolation ",
" does not intervene in the convergence behaviour of the scheme.",
" ",
" ",
" ",
" 6. Right hand side of wave equation read directly from file: the",
" typical use case specifies not the time function (source pulse)",
" appearing in the point defect model of an isotropic point radiator",
" (multiplying a spatial delta on the RHS of the the constitutive law,",
" relating rate of change of pressure to rate of change of volume), but",
" rather the pulse produced at a given distance from the source point in",
" a homogeneous medium. This is a convenient and meaningful calibration,",
" but for some purposes it may be more appropriate to specify the source",
" pulse directly. To enable this option, simply specify",
" ",
" refdist = 0.0",
" ",
" with other inputs as in the typical use case.",
" ",
" ",
" 7. Ricker pulse with given peak frequency: to produce a Ricker pulse",
" (in 3D), of specified peak frequency and unit amplitude at specified distance",
" from source point: inputs as in source specification in typical use",
"    case, with these amendments: ",
" ",
" omit source keyword",
" fpeak = [float] [default = 0.01 KHz] (also included in PML inputs) ",
" ",
" The reference distance specified by keyword refdist must be positive in this",
" case. The pulse produced in 2D is the 3/2-derivative of a Gaussian, rather",
" than a Ricker. ",
" ",
" ",
" ",
" 8. Override of computed time steps:",
" ",
" nt   = <int> [Default: computed from hdrfile, cfl condition]",
" dt   = <float> [Default: computed from hdrfile, cfl condition]",
" ",
"  If present, nt and dt override the number of time samples ",
"  and time step computed from hdrfile and cfl, for BOTH",
"  internal simulator time stepping AND output traces - i.e.",
"  if this option is chosen, then output traces will have",
"  nt samples and time step dt, overriding the values from  ",
"  the input header file (though the source and receiver locations",
"  remain as determined by the input header file). Note that ",
"  either neither or both must be set. This option is ",
"  useful mostly for convergence studies, for precise",
"  control of time step. No stability checking is",
"  performed by the code in this case, so ensuring stability",
"  is entirely up to the user - caveat emptor! ",
" ",
" ",
" ",
" Notes on Modeling with IWAVE ",
"  ",
"  ",
" 1. Parameter Parsing: Parameters are specified as key=value pairs, as in SU and",
" similar applications. String parameter values representing file names",
" can be either names of files in the working directory, or relative or",
" fully qualified pathnames. Any line not containing the separator",
" character (by default, the equals sign =) is treated as a",
" comment. Whitespace is ignored, including whitespace between key and",
" value strings and the separator (=). Key and value strings follow",
" regular expression syntax - in particular, key or value strings with",
" embedded whitespace must be quoted. Inline text separated from key or",
" value strings by whitespace is treated as comment. Lines can be",
  " commented out by quoting them. The typical parameter file ",
" asg/include/typical.par",
" illustrates some of these features. For further information about",
" parameter parsing in IWAVE, see IWAVE base package",
" documentation.   ",
" ",
" ",
" 2. Parallelism: IWAVE options include serial (single thread) execution,",
" and both distributed (MPI) and thread-based (OpenMP) parallelism. For",
" more on installation of these options, see the IWAVE install and build notes.",
" ",
" Due to the structure of finite difference schemes, the more useful sort",
" of parallelism is distributed, via domain decomposition. In IWAVE, domain",
" decomposition is defined by specifying the number of domains along ",
" each coordinate axis. The grids are divided as evenly as possible into subgrids -",
" the code currently makes no attempt to account for load imbalance,",
" for instance, between PML and non-PML subgrids. Can only be > 1 in ",
" any direction if MPI parallelism is enabled. Number of MPI processes",
" specified on command line must be at least total number of domains =",
" mpi_np1*mpi_np2*mpi_np3. ",
" ",
" For more on invoking these options, see the Typical Use notes, above.",
" ",
" ",
" 3. Survey simulation: IWAVE modeling applications simulate one",
" shot at a time. To simulate multiple shots, simply supply an output",
" file, identified by the datafile parameter, or a prototype header",
" file, containing the appropriate trace headers, sorted in shot order.",
" Each shot will be simulated and overwritten on the output file traces",
" in the order of appearance in the file.   ",
"  ",
"  ",
" 4. Internal units - chosen to conform to unit choices made, explicitly",
" or implicitly, in the SEGY standard.length = m, time = ms, mass = kg. Note",
" that frequency should be specified in KHz = cycles/ms, rather than Hz. ",
" Other consistent derived units and typical ranges for sedimentary",
" rocks are  velocities = O(1e+0) m/ms, bulk moduli =",
" O(1.e+3 - 1.e+4) MPa, densities = O(1.e+3) kg/m^3. ",
" All data must be scaled to these units before or during input. Note",
" that an IWAVE extension to RSF allows the user to change metric units",
" 'on the fly', thus avoiding the need to preprocess data files. To",
" scale RSF data during input, use the scale keyword to specify a power",
" of 10 scaling factor. For example, if velocity samples are in m/s, add",
" 'scale=-3' to the RSF header file for velocity, so that the data is",
" recorded internally in the expected m/ms units.    ",
" ",
" ",
" 5. Axes: Numbers (rather than letters) describe axes. We use the RSF/SEP ",
" system numbering (starting with 1). The identity of the axes (which one ",
" is depth, etc.) is vital to establish the correct sampling of spatial ",
" fields to SEGY trace output. IWAVE's default axis assignment is 1=z, ",
" (fastest axis), 2=x, 3=y. However, another IWAVE extension to RSF allows ",
" the user to override these defaults. If for example the axis ordering for ",
" density is 1=x (fastest axis), 2=y, and 3=z (as is conventional for GOCAD ",
" and many other applications), simply add \"x_axis=1\", \"y_axis=2\", and ",
" \"z_axis=3\" to the RSF header file. This facility allows the user to avoid",
" transposing data files.",
" ",
" Also note that only the information corresponding to active axes need be ",
" present in RSF header files. 1D simulation requires only the specification ",
" of axis 1 quantities,",
" 2D only axes 1 and 2.",
"   ",
" ",
" 6. Word order: IWAVE uses the RSF data_format keyword to control input word order",
" for material parameter data. Two choices are provided: native_float and xdr_float.",
" Since XDR word order is IEEE big endian, the choice data_format=xdr_float permits",
" big endian data to be read on little endian machines. The cost of the XDR stream",
" operations, and the proportion of cycles spent reading material parameter data,",
" are both so small in a typical 3D simulation that we strongly recommend that all ",
" material parameter data be XDR encoded.",
" ",
" Likewise, IWAVE's trace i/o package uses SU i/o functions, hence inherits SU's ",
" choice of XDR or native floating point word order. This choice is made at compile ",
" time, rather than run time as with RSF, via the SUXDR compiler directive (see the",
" IWAVE installation notes for more information).",
" ",
  "   ",
" 7. Source Scaling: IWAVE uses the source amplitude calibration proposed by",
" the SEAM project, that is, the source is Scaled so that in 3D,",
" the propagating wave matches the specified wavelet in reduced",
" time, for a specified propagation velocity and buoyancy and at",
" a specified distance from the source. ",
" "
" Source is implemented as a right hand side in the pressure update equation,",
" that is, as a constitutive law defect. Uses a discrete spatial delta multiplied",
" by a source wavelet (time function). This naive representation of a point",
" isotropic radiator is only moderately accurate, but suffices for many purposes.",
" Gives pressure field RMS errors of (at most) a few percent when",
" other sources of error are properly controlled. The discrete spatial",
" delta is represented by adjoint interpolation.",
"       ",
" ",
" 8. Output data paths: asg.x writes pressure traces to the file specified",
" by the datafile parameter. This filename can be a simple string, in which",
" case it is interpreted as the ",
" name of a file in the working directory, or a relative or fully qualified ",
" pathname. Verbose output (switched by the dump flags) is directed to a file ",
" named \"cout[rk].txt\" in the working directory, where [rk] is a fixed-width ",
" representation of process rank. Thus in serial execution verbose output ",
" goes to cout0.txt.",
" ",
"   ",
" 9. RSF implementation: IWAVE apps expect material parameters and other",
" grid functions to be stored in RSF format files. RSF is an update of the",
" SEP 77 file format, created by Sergey Fomel. For the basics on",
" RSF, see http://www.reproducibility.org/wiki/Guide_to_RSF_file_format",
" ",
" IWAVE defines a few useful extensions to the basic RSF format. Extra keywords ",
" define the physical significance of the grid axis (is the fastest",
" axis z or x or y?) and parameter value scaling. See IWAVE grid package ",
" documentation for information on the IWAVE extensions to the RSF format. ",
" IWAVE imposes one limitation on hypercubical grids, defined by RSF attributes", 
" the origin of the physical coordinate system must be a grid point, in order",
" that various sampling and resampling functions perform properly.",
" ",
" Note: velocity RSF data structure defines the grid of the pressure",
" and bulk modulus fields. The grids for the (particle) velocity",
" components and buoyancies are obtained by shifting this grid by a",
" half-step in each coordinate direction. The spatial dimension of the",
" simulation is that defined by the velocity (pressure, bulk modulus)",
" grid, i.e. the greatest int dim (between 1 and 3 inclusive) for",
" which the RSF parameter ndim is greater than 1, and the spatial",
" steps used in the simulation are the dk, k=1,...,dim. The density",
" domain defined by the density RSF data structure may differ from the",
" velocity domain, but the density is assumed to have the same",
" grid steps (d's), and the code does not test this assumption.",
" The density reciprocal (buoyancy) is interpolated linearly onto the",
" shifted particle velocity grids, and extrapolated as constant, first",
" in y, then in x, then in z, if necessary.",
" ",
"  ",
" 10. Grid Sampling: the \"greatest integer\" option extracts (or inserts,",
" for the adjoint case) the sample at the gridpoint whose coordinates are",
" closest and less than those of the nominal sample",
" point. Thus sampling the pressure field interior to the row of grid",
" cells to the right (i.e. with greater normal coordinate) of a",
" reflecting boundary will produce zero samples, and inserting a",
" source point using the simple isotropic point source option in this",
" layer will also produce zero energy input. Note that this occurs",
" when z=0 defines the reflecting boundary, the simulation domain is",
" in z>0, and the source or receiver z-coordinate is less than the",
" spatial step in z. To obtain nontrivial results with sources and/or",
" receivers in the first row of grid cells to the right of a",
" reflecting boundary, always use multilinear sampling (sampord=1).",
" ",
" ",
" Additional Notes on the Acoustic Modeling Application",
" ",
" 1. Stability: The code checks that the input velocity does not exceed the bounds",
" specified by the optional parameters cmax and cmin. HOWEVER it is by",
" now well-known that for this type of scheme, the CFL condition is not",
" sufficient to guarantee stability.  In fact stabiliy depends also on",
" the density field, markedly so when the density has",
" discontinuities. Gustafsson and Wahlund, SIAM J. Sci. Comp. 26 (2004),",
" pp.  272-293 derived a necessary and sufficient stability condition,",
" which we have implemented: our internally defined time step conforms",
" to this criterion, and is stable. There is unfortunately no simple a",
" priori calculation to yield a stable step, but this is only a concern",
" if the user insists on overriding the internally computed step (see  alternate )",
" - in that case, as we said, caveat emptor!   ",
" ",
" ",
" 2. Order specification: parameters scheme_phys, scheme_npml",
" encode time and space orders. Schemes specified independently for physical",
" domain (scheme_phys) and PML layers (scheme_npml). Valid choices are",
" 22: 2nd order in space and time, 24: 2nd order in time, 4th order in",
" space (default choice), 210: 2nd order in time, 10th order in space,",
" 2k: 2nd order in time, 2kth order in space.  For the 2k choice, the",
" half-orders k_phys, k_npml must also be specified. NOTE: in release 1.0",
" (November 2009), the 2D 210 scheme is not functional - calling it will",
" result in an error code being returned. The 1D and 3D versions of 210",
" are completely functional, on the other hand. To obtain 10th order (or",
" any other order higher than 4) in 2D space, use the 2k construction.",
" ",
" Paramters k_phys, k_npml determine spatial orders for 2k schemes.",
" Valid range = 1 through 7. Required if scheme_phys or scheme_npml = 2k,",
" no effect otherwise.  NOTE: 22, 24, and 210 schemes are inlined, whereas 2k",
" schemes employ an internal loop over the stencil. Thus for example",
" 210 tends to be up to 2x faster than 2k with k=5.  NOTE: it is NOT",
" NECESSARY to use the same orders in the interior and in the pml",
" layers. A high order scheme in the physical can be used in",
" combination with the 22 scheme in the pml layers for example, for",
" some small boost in speed",
" ",
" ",
" 3. Limits:",
" ",
" cmax = [float] [Default=5 m/ms] maximum permitted compressional",
" velocity used to compute simulation time step via CFL-type stability",
" condition [HOWEVER SEE NOTE BELOW REGARDING MORE STRINGENT STABILITY",
" CHECK]. Input velocity checked against this limit, error returned if",
" > cmax at any grid point.",
" ",
" cmin = [float] [Default=0.2*cmax] minimum permitted compressional",
" velocity, functions exclusively as a sanity check on the input",
" parameters in this implementation. Since simulation spatial grid is",
" directly specified, not used to verify any von N.  GP/WL accuracy",
" criterion or the like.Input velocity checked against this limit,",
" error returned if < cmin at any grid point.",
" ",
" cfl = [float] CFL number [Default=0.4] The default",
" value is stable for all schemes in the currently implemented family",
" (2-2 through 2-12, 1-3D).  More conservative choices may result in",
" less grid dispersion.",
" ",
" ",
" 4. PML absorbing boundary conditions: defined by layer thicknesses and",
" absorption amplitude. Parameters nl1, nr1,... specify thicknesses of left",
" and right absorbing layers along each axis, in wavelengths at nominal peak",
" frequency (fpeak) and maximum velocity (cmax). For example, to specify free",
" surface at z=0, absorbing conditions at all other boundaries, in a 2D simulation,",
" reasonable choices would be nl1=0.0, nr1=0.5, nl2=0.5, nr2=0.5 (assuming that z",
" is axis 1 and x is axis 2).",
" ",
" ",
  NULL };
