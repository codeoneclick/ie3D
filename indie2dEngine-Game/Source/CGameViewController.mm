//
//  CGameViewController.m
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameViewController.h"
#include "COGLWindow_iOS.h"
#include "CGame.h"
#include "CGameRootTransition.h"
#include "CCommonOS.h"
#include "CModel.h"
#include "CCamera.h"
#include "CLight.h"


@interface CGameViewController ()

@property (weak, nonatomic) IBOutlet COGLWindow_iOS *m_glWindow;
@property (unsafe_unretained, nonatomic) CGameRootTransition* m_transition;
@end

@implementation CGameViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        
    }
    return self;
}

size_t ICCurlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	const size_t sizeInBytes = size * nmemb;
    NSData *data = [[NSData alloc] initWithBytes:ptr length:sizeInBytes];
    NSString *myString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"%@", myString);
	return sizeInBytes;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    CURL *curl;
    CURLcode res;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    curl = curl_easy_init();
    if(curl)
    {
        struct curl_slist *headers = NULL;
        
        //curl_slist_append(headers, "Accept: application/json");
        //curl_slist_append(headers, "Content-Type: application/json");
        //curl_slist_append(headers, "charsets: utf-8");
       
        headers = curl_slist_append(headers, "Accept:  application/xml");
        headers = curl_slist_append(headers, "Content-Type: text/xml");
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.w3schools.com/xml/note.xml");
        curl_easy_setopt(curl, CURLOPT_PROXY, "http://proxy.kha.gameloft.org:3128");
        curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, "sergey.sergeev");
        curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, "1986Click@0509");
        curl_easy_setopt(curl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ICCurlWriteCallback);
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        
#ifdef SKIP_PEER_VERIFICATION
        /*
         * If you want to connect to a site who isn't using a certificate that is
         * signed by one of the certs in the CA bundle you have, you can skip the
         * verification of the server's certificate. This makes the connection
         * A LOT LESS SECURE.
         *
         * If you have a CA cert for the server stored someplace else than in the
         * default bundle, then the CURLOPT_CAPATH option might come handy for
         * you.
         */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
        
#ifdef SKIP_HOSTNAME_VERIFICATION
        /*
         * If the site you're connecting to uses a different host name that what
         * they have mentioned in their server certificate's commonName (or
         * subjectAltName) fields, libcurl will refuse to connect. You can skip
         * this check, but this will make the connection less secure.
         */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
        long statuscode = 0;
        if (res == CURLE_OK)
        {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statuscode);
        }

        
        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
    
    self.m_transition = new CGameRootTransition((__bridge void*)_m_glWindow);
    std::shared_ptr<CCamera> camera = self.m_transition->CreateCamera(60.0f,
                                    0.1f,
                                    256.0f,
                                    glm::vec4(0.0f, 0.0f, Get_ScreenWidth(), Get_ScreenHeight()));
    camera->Set_Position(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->Set_LookAt(glm::vec3(32.0f, 0.0f, 32.0f));
    camera->Set_Distance(8.0f);
    camera->Set_Height(8.0f);
    std::shared_ptr<CLight> light = self.m_transition->CreateLight();
    light->Set_Position(glm::vec3(32.0f, 32.0f, 32.0f));
    self.m_transition->Set_Camera(camera);
    self.m_transition->Set_Light(light);
    std::shared_ptr<CModel> model = self.m_transition->CreateModel("model.xml");
    model->Set_Position(glm::vec3(32.0f, 0.0f, 32.0f));
    self.m_transition->InsertModel(model);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end
