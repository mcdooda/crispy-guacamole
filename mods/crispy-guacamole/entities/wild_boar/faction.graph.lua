script {
  links = {
    {
      1,
      1,
      3,
      1,
    },
    {
      2,
      1,
      1,
      1,
    },
  },
  nodes = {
    {
      name = 'components/factioncomponent',
    },
    {
      loadArguments = {
        'wild life',
      },
      name = 'string',
    },
    {
      loadArguments = {
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
  },
}