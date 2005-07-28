from rsfproj import *
import cluster

def param(par):
    p  = ' '
    p += ' readwrite=y'
    if(par.has_key('verb')):  p += ' verb='  +     par['verb']
    if(par.has_key('nrmax')): p += ' nrmax=' + str(par['nrmax'])
    if(par.has_key('dtmax')): p += ' dtmax=' + str(par['dtmax'])
    if(par.has_key('tmx')):   p += ' tmx='   + str(par['tmx'])
    if(par.has_key('tmy')):   p += ' tmy='   + str(par['tmy'])
    if(par.has_key('pmx')):   p += ' pmx='   + str(par['pmx'])
    if(par.has_key('pmy')):   p += ' pmy='   + str(par['pmy'])
    p += ' '
    return p

def freqs(par):
    f  = ' '
    if(par.has_key('nw')): f += ' nw=' + str(par['nw'])
    if(par.has_key('ow')): f += ' ow=' + str(par['ow'])
    if(par.has_key('dw')): f += ' dw=' + str(par['dw'])
    f += ' '
    return f

def wflds(wfld,cmps,par):
    Flow(wfld,cmps,
         '''
         fft1 inv=n |
         window squeeze=n n1=%(nw)d min1=%(ow)g |
         transp plane=13 memsize=500 |
         spray axis=2 n=1 o=0 d=1 |
         put label1=mx label2=my label3=hx label4=w
         ''' % par )
    
def datum(wfl1,slow,wfl0,par):
    Flow(wfl1,[wfl0,slow],
         '''
         camig mode=d inv=n %s
         slo=${SOURCES[1]}
         ''' % param(par))

def model(data,slow,imag,par):
    Flow(data,[imag,slow],
         '''
         camig mode=m inv=y %s %s
         slo=${SOURCES[1]}
         ''' % (param(par),freqs(par)))
    
def image(imag,slow,data,par):
    Flow(imag,[data,slow],
         '''
         camig mode=m inv=n %s
         slo=${SOURCES[1]}
         ''' % param(par))

#def cimage(imag,slow,data,par):
#    Flow(imag,[data,slow],
#         '''
#         camig mode=m inv=n %s
#         slo=${SOURCES[1]}
#         <   ${SOURCES[0]}
#         ''' % param(par), stdin=0)

# ------------------------------------------------------------

# generate cluster execution commands
def script(EDIR,job,imag,slow,wfld,par,ngroup,nfreqs):
    mycom = 'rm ' + EDIR + '/' + job
    os.system(mycom)

    mycom = 'cp ' + bindir + '/sfcamig' + ' '+ EDIR
    os.system(mycom)
    
    allk = map(lambda x: '%03d' % x,range(ngroup))
    for k in allk:
        _i = '_' + imag + '.' + k + '.rsf'
        _w = '_' + wfld + '.' + k + '.rsf'
        _v = '_' + slow + '.rsf'

        mycom  = 'sfcamig'
        mycom += param(par)
        mycom += ' <'    + _w
        mycom += ' slo=' + _v
        mycom += ' >'    + _i
        mycom += ' datapath=' + EDIR
        mycom = 'echo "' + mycom + '" >>' + EDIR + '/' + job
        os.system(mycom)

def execute(EDIR,JOB,ngroup,nfreqs,imag,slow,wfld,par):
    script = EDIR + '/' + JOB

    f = open(script,'w')

    allk = map(lambda x: '%03d' % x,range(ngroup))
    for k in allk:
        _i = '_' + imag + '.' + k + '.rsf'
        _w = '_' + wfld + '.' + k + '.rsf'
        _v = '_' + slow + '.rsf'

        mycom  = 'sfcamig'
        mycom += param(par)
        mycom += ' <'    + _w
        mycom += ' slo=' + _v
        mycom += ' >'    + _i
        mycom += ' datapath=' + EDIR
        f.write(mycom+'\n')

    f.close()

bsub = WhereIs('bsub')

def run(img,wfl,slo,par,clspar):
    if bsub:
        cluster.sprd(wfl,clspar['EDIR'],4,clspar['fgroup'],clspar['felem'],clspar['fjump'])
        cluster.copy(slo,clspar['EDIR'])
        
        JOB = 'SSJOB' + '-' + slo
        
        script(clspar['EDIR'],JOB,img,slo,wfl,par,
               clspar['fgroup'],
               clspar['felem'])
        
        cluster.launch(clspar['EDIR'],JOB,
                       clspar['fgroup'],
                       clspar['time'],
                       clspar['queue'],
                       clspar['project'])
#
#        Command(        clspar['EDIR']+'/'+JOB, None,
#                execute(clspar['EDIR']  ,  JOB,
#                        clspar['fgroup'],
#                        clspar['felem'],
#                        img,slo,wfl,par),stdout=0)
#        Command(               clspar['EDIR']+'/'+JOB,None,
#                cluster.submit(clspar['EDIR']  ,  JOB,
#                               clspar['fgroup'],
#                               clspar['time'],
#                               clspar['queue'],
#                               clspar['project']))
#        
        # submit to cluster using 'bsub < $JOB'
        
        cluster.summ(img,clspar['EDIR'],clspar['fgroup'])
    else:
        image(img,slo,wfl,par)

        
