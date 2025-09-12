package com.nomad;

import org.springframework.context.annotation.Bean;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.system.ApplicationHome;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.client.RestTemplate;

@SpringBootApplication
public class Application {
    public static void main(String[] args) {
         var applicationHome = new ApplicationHome(Application.class);
         System.setProperty("app.home", applicationHome.getDir().getParent());
         System.setProperty("spring.config.location", "classpath:application.yml");
         System.setProperty("spring.config.additional-location", "${app.home}/conf/application.yml");
         SpringApplication application = new SpringApplication(Application.class);
         application.run(args);
    }

    @Bean
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }
}
