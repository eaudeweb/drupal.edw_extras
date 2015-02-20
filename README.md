# Eau de Web utility modules

This Drupal module contains a set of reusable functionalities in various projects

# Installing

Download latest release from Github 'releases' screen

## Components

### Caching framework


### Database framework


### Security framework

  Security module generates a security file safely stored in web server and you can set-up a CRON job to notify when security check fails.
  
  ```
     drush edw-security-generate > /path/to/file/outside/document/root/security.file
     
     drush edw-security-check /path/to/file/outside/document/root/security.file
  ```

  * If the security check fails, the drush process exits with error. This would trigger cron to send an e-mail report
  * If the security check passed, nothing happens.
  
  Currently implemented functionality checks the integrity of the ```registry``` table, known to be prone to hacks
